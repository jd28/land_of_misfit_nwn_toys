#pragma once

#include "ResourceLocator.h"
#include "NwnDefines.h"
#include "api/all.h"

#include <string>
#include <vector>

class ResourceManager {
    static std::string resref_cache;
    std::vector<NwnResType> cache;
    ResourceLocator locator;
    size_t memory_used = 0;
    size_t memory_usage_limit = 0;

public:
    // Add a static container.  These are containers whose contents never change.
    // E,g. Erf, Zip, Key/Bif, etc.
    void AddStatic(Container *cont) { locator.AddStatic(cont); }

    // Add a dynamic container.  These are containers that can load resources from
    // any location.
    void AddDynamic(Container* cont) { locator.AddDynamic(cont); }

    // Allocates a chunk of memory.  The actual size allocated is dependent on
    // the flags passed in.  This is due to the vagaries of some NWN resource
    // types.
    char *Allocate(size_t size, bool flag1, bool flag2);

    // Deallocate a chunk of memory.
    void Deallocate(char *data, bool flag1, bool flag2);

    // Find and returns the data for a resource.
    char *Demand(CRes *cRes);

    // Bridge between our resource manager and nwserver CExoResMan.  Basically a thin wrapper
    // around ResourceLocater::Query
    int GetKeyEntry(const CResRef& resref, uint16_t type, CExoKeyTable **table, CKeyTableEntry **v2);

    // Get the amount of memory that is currently allocated.
    size_t GetMemoryUsed() { return memory_used; }

    // Determines if we alread have a container of a particular name.  Note that
    // for Key/Bif, Mod, Hak, and Override the name will be the aliased name.
    // i.e. 'HAK:myhak'
    bool HasContainer(const char* name) const { return locator.HasContainer(name); };

    // Checks if a particular resource is one that is cached.
    bool IsCached(NwnResType rtpye);

    // Sets the types that will be cached and (currently) never freed.
    void SetCachedTypes(std::vector<NwnResType> cache_rtypes);

    // Sets the limit for memory useage; Not currently implemented.
    void SetMemoryUsageLimit(size_t size) { memory_usage_limit = size; }
};
