#include "NWNXResMan.h"
#include "pluginlink.h"

#include "containers/Erf.h"
#include "containers/Key.h"
#include "containers/ResmanDirectory.h"
#include "containers/Zip.h"

#include <boost/algorithm/string.hpp>
#include <sys/stat.h>
#include <stdio.h>
#include <boost/algorithm/string/predicate.hpp>

extern CNWNXResMan resman;

static int AddContainerService(uintptr_t event) {
    ResManAddContainerService *e = reinterpret_cast<ResManAddContainerService*>(&event);
    if(!e->container) {
        return 0;
    }
    resman.resources.AddDynamic(reinterpret_cast<Container*>(e->container));
    return 1;
}

CNWNXResMan::CNWNXResMan()
{
    confKey = "RESOURCEMANAGER";
    warnMissing = false;
    debugCRes = false;
}

CNWNXResMan::~CNWNXResMan()
{
}


bool CNWNXResMan::OnCreate(gline *config, const char *LogDir)
{
    char log[MAXPATH];

    sprintf(log, "%s/nwnx_resman.txt", LogDir);
    if (!CNWNXBase::OnCreate(config, log))
        return false;

    WriteLogHeader();

    hAddContainer = CreateHookableEvent(EVENT_RESMAN_ADD_CONTAINER);
    hAddContainerService = CreateServiceFunction(SERVICE_RESMAN_ADD_CONTAINER, AddContainerService);

    // Must be called after event hooks are created and before
    // hooks.
    LoadConfiguration();
    HookFunctions();
    HookResman();

    //erfs.reserve(100);

    return true;
}

char* CNWNXResMan::OnRequest(char* gameObject, char* Request, char* Parameters)
{
    return NULL;
}

bool CNWNXResMan::OnRelease()
{
    Log(0, "o Shutdown.\n");
    return true;
}

void CNWNXResMan::DumpResStruct(CRes *cRes)
{
    Log(4, "CRes = %p\n", cRes);
    Log(4, "  - m_nDemands = %d\n", cRes->m_nDemands);
    Log(4, "  - m_nRequests = %d\n", cRes->m_nRequests);
    Log(4, "  - m_nID = %08lX\n", cRes->m_nID);
    Log(4, "  - m_pResource = %08lx\n", cRes->m_pResource);
    Log(4, "  - m_nSize = %d\n", cRes->m_nSize);
    if (cRes->m_pKeyEntry) {
        CKeyTableEntry *key = cRes->m_pKeyEntry;
        char buf[17] = {0};
        strncpy(buf, key->m_cResRef.m_resRef, 16);
        Log(4, "  - m_sName = %s\n", buf);
    }
    Log(4, "  - flags = %x\n", cRes->field_8);
    Log(4, "  - m_status = %d\n", cRes->m_status);
    Log(4, "  - m_pKeyEntry = %08lx\n", cRes->m_pKeyEntry);
}

CKeyTableEntry *CNWNXResMan::CreateKeyTableEntry(const CResRef &resRef, NwnResType resType)
{
    CKeyTableEntry *key = new CKeyTableEntry;
    key->m_pRes = NULL;
    key->m_cResRef = CResRef(resRef);
    key->m_nID = -1;
    key->m_nRefCount = 0;
    key->m_nType = resType;
    return key;
}

bool CNWNXResMan::AddContainer(const char *path, const char* name)
{
    ResManAddContainerEvent event{ path };
    int notifyRet = NotifyEventHooks(hAddContainer, (uintptr_t)&event);
    if(notifyRet) {
        return true;
    } else {
        Container *c = nullptr;
        struct stat info;
        if (!path || stat(path, &info)) {
            return false;
        }
        if(boost::algorithm::ends_with(path, "hak")
                || boost::algorithm::ends_with(path, "mod")
                || boost::algorithm::ends_with(path, "erf")
                || boost::algorithm::ends_with(path, "nwm")
                || boost::algorithm::ends_with(path, "sav"))
        {
            std::string p = path;
            p += ".zip";
            if (stat(p.c_str(), &info) == 0) {
                c = new ZipContainer(p.c_str(), name);
            } else {
                c = new Erf(path, name);
            }
        } else if(boost::algorithm::ends_with(path, "key")) {
            std::string p = path;
            p += ".zip";
            if (stat(p.c_str(), &info) == 0) {
                c = new ZipContainer(p.c_str(), name);
            } else {
                c = new Key(path, name);
            }
        }
        if(c) {
            resources.AddStatic(c);
        }
    }

    return true;
}

void CNWNXResMan::WriteLogHeader()
{
    // write copy information to the log file
    Log(0, "NWNX Resource Manager 2.0 for Linux.\n");
    Log(0, "(c) 2005 by Ingmar Stieger (papillon@nwnx.org)\n");
    Log(0, "(c) 2006 by dumbo (dumbo@nm.ru)\n");
    Log(0, "(c) 2006-2010 by virusman (virusman@virusman.ru)\n");
    Log(0, "(c) 2015 by jmd (jmd2028@gmail.com)\n");
}

void CNWNXResMan::LoadConfiguration()
{
    gline& conf = *nwnxConfig;
    if (conf.exists(confKey)) {
        warnMissing = atoi(conf[confKey]["warn_missing"].c_str());
        Log(0, "o Logging missing resources: %d\n", warnMissing);

        debugCRes = atoi(conf[confKey]["debug_cres"].c_str());
        Log(0, "o Debugging CRes construction and destruction: %d\n", debugCRes);

        std::string sourcePath = conf[confKey]["SourcePath"];
        if (sourcePath.length() > 0) {
            auto c = new ResmanDirectoryContainer(sourcePath, "RESMAN");
            resources.AddDynamic(c);
        }

        std::string cache_size = conf[confKey]["memory_usage_limit"];
        size_t csize = 0;
        if(cache_size.size() > 0) {
            csize = std::atoi(cache_size.c_str());
            Log(0, "o Setting memory usage limit: %d\n", csize);
        }
        resources.SetMemoryUsageLimit(csize);

        std::string cache_types = conf[confKey]["cache"];
        if(cache_types.size() > 0) {
            std::vector<std::string> exts;
            std::vector<NwnResType> cache_rtypes;
            boost::split(exts, cache_types, boost::is_any_of(","));
            exts.push_back("ncs");
            for(auto& e : exts) {
                NwnResType rtype = NwnGetResTypeFromExtension(e.c_str());
                if(rtype != NwnResType_Unknown) {
                    Log(0, "o Caching %s resources.\n", e.c_str());
                    cache_rtypes.push_back(rtype);
                }
            }
            if(cache_rtypes.size() > 0) {
                resources.SetCachedTypes(cache_rtypes);
            }
        }
    }
}
