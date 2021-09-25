#include "NWNXResMan.h"
#include "NWNXApi.h"
#include "containers/Erf.h"
#include "containers/Key.h"
#include "containers/Directory.h"
#include <sys/stat.h>
#include <boost/algorithm/string.hpp>
#include "NwnDefines.h"
#include <tuple>

extern CNWNXResMan resman;

int (*CExoResMan__AddResourceImageFile)(CExoResMan*, CExoString const &, unsigned char *) = reinterpret_cast<decltype(CExoResMan__AddResourceImageFile)>(0x082b3800);
int CExoResMan__AddResourceImageFile_hook(CExoResMan*a, CExoString const &b, unsigned char *c) {
    resman.Log(0, "AddResourceImageFile: %s\n", b.Text);
    return CExoResMan__AddResourceImageFile(a, b, c);
}

int (*CExoResMan__AddEncapsulatedResourceFile)(CExoResMan*, CExoString const &) = reinterpret_cast<decltype(CExoResMan__AddEncapsulatedResourceFile)>(0x082b37e4);
int CExoResMan__AddEncapsulatedResourceFile_Hook(CExoResMan* rm, const CExoString& erf) {
    CExoString file = g_pExoBase->AliasList->ResolveFileName(erf, NwnResType::NwnResType_MOD);
    if (resman.AddContainer(file.Text, erf.Text)) { goto is_found; }
    file = g_pExoBase->AliasList->ResolveFileName(erf, NwnResType::NwnResType_HAK);
    if (resman.AddContainer(file.Text, erf.Text)) { goto is_found; }
    file = g_pExoBase->AliasList->ResolveFileName(erf, NwnResType::NwnResType_ERF);
    if (resman.AddContainer(file.Text, erf.Text)) { goto is_found; }
    file = g_pExoBase->AliasList->ResolveFileName(erf, NwnResType::NwnResType_NWM);
    if (resman.AddContainer(file.Text, erf.Text)) { goto is_found; }
    file = g_pExoBase->AliasList->ResolveFileName(erf, NwnResType::NwnResType_SAV);
    if (resman.AddContainer(file.Text, erf.Text)) { goto is_found; }
    return 0;
is_found:
    return 1;
}

int (*CExoResMan__AddFixedKeyTableFile)(CExoResMan*, CExoString const &) = reinterpret_cast<decltype(CExoResMan__AddFixedKeyTableFile)>(0x082b381c);
int CExoResMan__AddFixedKeyTableFile_Hook(CExoResMan* rm, const CExoString& key) {
    std::vector<std::string> strs;
    boost::split(strs, key.Text, boost::is_any_of(";"));
    std::reverse(strs.begin(), strs.end());
    for(const auto& f : strs) {
        // This how nwserver does it...
        if(strcasecmp(f.c_str(), "hd0:xp1") == 0) {
             g_pExoBase->field_14 |= 1u;
        } else if(strcasecmp(f.c_str(), "hd0:xp2") == 0) {
            g_pExoBase->field_14 |= 2u;
        } else if(strcasecmp(f.c_str(), "hd0:xp3") == 0) {
            //g_pExoBase->field_14 |= 4u;
        } else if(strcasecmp(f.c_str(), "hd0:xp4") == 0) {
            g_pExoBase->field_14 |= 8u;
        } else if(strcasecmp(f.c_str(), "hd0:xp5") == 0) {
            g_pExoBase->field_14 |= 10u;
        } else if(strcasecmp(f.c_str(), "hd0:xp6") == 0) {
            g_pExoBase->field_14 |= 20u;
        }

        CExoString file = g_pExoBase->AliasList->ResolveFileName(f.c_str(), NwnResType::NwnResType_KEY);
        if(resman.AddContainer(file.Text, f.c_str())) {
            ;
        }
    }
    return 1;
}

int (*CExoResMan__RemoveEncapsulatedResourceFile)(CExoResMan*, CExoString const &) = reinterpret_cast<decltype(CExoResMan__RemoveEncapsulatedResourceFile)>(0x082b3854);
int CExoResMan__RemoveEncapsulatedResourceFile_hook(CExoResMan* rm, CExoString const &erf) {
    if(!resman.resources.HasContainer(erf.Text)) {
        resman.Log(0, "Removing ERF: %s\n", erf.Text);
        return CExoResMan__RemoveEncapsulatedResourceFile(rm, erf);
    }
    return 0;
}

int (*CExoResMan__AddResourceDirectory)(CExoResMan*, CExoString const &) = reinterpret_cast<decltype(CExoResMan__AddResourceDirectory)>(0x082b3838);
int CExoResMan__AddResourceDirectory_hook(CExoResMan* rm, CExoString const &dir)
{
    if(dir.Text) {
        if (strcmp("OVERRIDE:", dir.Text) == 0) {
            CExoString alias = g_pExoBase->AliasList->ResolveFileName(dir.Text, 0xFFFF);
            resman.Log(0, "Attempting to add directory: %s\n", alias.Text);
            DirectoryContainer *c = new DirectoryContainer(alias.Text, dir.Text);
            resman.resources.AddDynamic(c);
            return 1;
        }
    }

    return CExoResMan__AddResourceDirectory(rm, dir);
}

unsigned long (*CNWSModule__LoadModuleStart)(CNWSModule*, CExoString, int) = reinterpret_cast<decltype(CNWSModule__LoadModuleStart)>(0x081b4a4c);
unsigned long CNWSModule__LoadModuleStart_hook(CNWSModule* mod, CExoString s, int i) {
    unsigned long res = CNWSModule__LoadModuleStart(mod, s, i);
    resman.Log(0, "Load module start return value: %d\n", res);
    return res;
}

int (*CExoResMan__FreeResourceData)(CExoResMan*, CRes *) = reinterpret_cast<decltype(CExoResMan__FreeResourceData)>(0x082b3990);
int CExoResMan__FreeResourceData_hook(CExoResMan* rm, CRes *res)
{
    if(res->m_pKeyEntry && resman.resources.IsCached((NwnResType)res->m_pKeyEntry->m_nType)) {
        resman.Log(4, "Skipping FreeResourceData %.*s, %d\n",
                   16,
                   res->m_pKeyEntry->m_cResRef.m_resRef,
                   (NwnResType)res->m_pKeyEntry->m_nType);

        res->m_pKeyEntry->m_pRes = nullptr;
        return 0;
    }
    return CExoResMan__FreeResourceData(rm, res);
}

void HookResman() {
    NX_HOOK(CExoResMan__FreeResourceData,
            CExoResMan__FreeResourceData,
            CExoResMan__FreeResourceData_hook,
            5);

    NX_HOOK(CExoResMan__AddResourceDirectory,
            CExoResMan__AddResourceDirectory,
            CExoResMan__AddResourceDirectory_hook,
            6);

    NX_HOOK(CExoResMan__AddResourceImageFile,
            CExoResMan__AddResourceImageFile,
            CExoResMan__AddResourceImageFile_hook,
            5);

    NX_HOOK(CExoResMan__AddEncapsulatedResourceFile,
            CExoResMan__AddEncapsulatedResourceFile,
            CExoResMan__AddEncapsulatedResourceFile_Hook,
            6);

    NX_HOOK(CExoResMan__AddFixedKeyTableFile,
            CExoResMan__AddFixedKeyTableFile,
            CExoResMan__AddFixedKeyTableFile_Hook,
            6);

    NX_HOOK(CExoResMan__RemoveEncapsulatedResourceFile,
            CExoResMan__RemoveEncapsulatedResourceFile,
            CExoResMan__RemoveEncapsulatedResourceFile_hook,
            5);
}
