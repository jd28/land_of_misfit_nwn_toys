#include "../NWNXResMan.h"
#include "ResourceManager.h"
#include "../util.h"
#include <algorithm>

extern CNWNXResMan resman;

std::string ResourceManager::resref_cache;

char* ResourceManager::Allocate(size_t size, bool flag1, bool flag2) {
    size_t final = size;
    char *data = nullptr;
    if(flag1) {
        data = new char[size+6];
        data += 6;
        final += 6;
    } else if (flag2) {
        data = new char[size+10];
        final += 10;
    } else {
        data = new char[size];
    }
    resman.Log(4, "    Allocated %d bytes.\n", final);
    return data;
}

void ResourceManager::Deallocate(char *data, bool flag1, bool flag2) {
    if(flag1) {
        data -= 6;
    }
    delete[] data;
}

void ResourceManager::SetCachedTypes(std::vector<NwnResType> cache_rtypes)
{
    cache = std::move(cache_rtypes);
}

bool ResourceManager::IsCached(NwnResType rtype)
{
    auto it = std::find(cache.begin(), cache.end(), rtype);
    return it != cache.end();
}

char* ResourceManager::Demand(CRes *cRes)
{
    if(!cRes || !cRes->m_pKeyEntry) { return 0; }
    bool is_cached = false;
    char *data = nullptr;
    size_t size = 0;
    resref_cache = resref_to_fname(cRes->m_pKeyEntry->m_cResRef, cRes->m_pKeyEntry->m_nType);
    ResourceEntry& loc = locator.Get(resref_cache);

    if(!loc.valid()) {
        resman.Log(4, "     Not in Cache Demand: resref: %.*s, type: %d\n", 16,
                   cRes->m_pKeyEntry->m_cResRef.m_resRef, cRes->m_pKeyEntry->m_nType);
        return data;
    }

    if(cRes->m_pResource) {
        if(loc.last_demand < loc.element.mtime) {
            resman.Log(0, "Freeing cached data\n");
            Deallocate((char*)cRes->m_pResource, !!cRes->field_1C, !!cRes->field_20);
            memory_used -= cRes->m_nSize;

            cRes->m_pResource = nullptr;
            cRes->m_nSize = 0;
        }
        data = (char*)cRes->m_pResource;
        size = cRes->m_nSize;
        is_cached = true;
    } else if(loc.cache) {
        if(loc.last_demand < loc.element.mtime) {
            resman.Log(0, "Freeing cached data\n");
            Deallocate(loc.cache, !!cRes->field_1C, !!cRes->field_20);
            memory_used -= cRes->m_nSize;
            loc.cache = nullptr;
        }
        cRes->m_pResource = data = loc.cache;
        size = cRes->m_nSize;
        is_cached = true;
    }

    if(!cRes->m_pResource) {
        cRes->field_8 |= 0x10;
        loc.last_demand = std::time(0);
        ++cRes->m_nDemands;

        data = Allocate(loc.element.size, !!cRes->field_1C, !!cRes->field_20);
        size = loc.container->Demand(resref_cache, loc.element, data);
        if(size != loc.element.size) {
            // If the container couldn't read all or read more than we expected
            // bail completely and return nothing.
            resman.Log(0, "Container %s reported differing size.\n", loc.container->name());
            Deallocate(data, !!cRes->field_1C, !!cRes->field_20);
            data = nullptr;
            size = 0;
        } else {
            cRes->m_pResource = data;
            cRes->m_nSize = loc.element.size;
            memory_used += loc.element.size;
        }

        if(IsCached((NwnResType)loc.key->m_nType)) {
            loc.cache = data;
        }

    }

    resman.Log(4, "     Demand Last: Container: %s, resref: %.*s, type: %d, size: %d, Cached: %d\n",
               loc.container->name(),
               16,
               loc.key->m_cResRef.m_resRef,
               loc.key->m_nType,
               size,
               is_cached);

    int res = 0;

    // I don't see how or why this stuff is necessary.
    if (cRes->m_pKeyEntry->m_nType == NwnResType_NCS) {
        CResNCS *t= (CResNCS*) cRes;
        t->m_bLoaded = 0;
    } else if (cRes->m_pKeyEntry->m_nType == NwnResType_2DA) {
        CRes2DA *t = (CRes2DA*) cRes;
        t->m_bLoaded = 0;
    }/* else if (cRes->m_pKeyEntry->m_nType == NwnResType_SET) {
        CResSET *t = (CResSET*) cRes;
        t->Loaded = 0;
        t->OnResourceServiced();
    } else if (cRes->m_pKeyEntry->m_nType == NwnResType_GIT
               || cRes->m_pKeyEntry->m_nType == NwnResType_ARE) {
        CResGFF *t = (CResGFF*) cRes;
        t->m_bLoaded = 0;
        t->OnResourceServiced();
        resman.Log(0, "GIT: %d\n", t->field_9C);
    }
    else {
    }*/

    res = cRes->vtbl->OnResourceServiced(cRes);
    cRes->field_8 &= 0xFFFFFFFB; // Clear released flag.
    if (res) {
        cRes->field_8 |= 4;
    }

    return data;
}

int ResourceManager::GetKeyEntry(const CResRef &resref, uint16_t type, CExoKeyTable **table, CKeyTableEntry **v2)
{
    const ResourceEntry& e = locator.Query(resref, type, table);
    *v2 = e.key;
    return e.valid() ? 1 : 0;
}
