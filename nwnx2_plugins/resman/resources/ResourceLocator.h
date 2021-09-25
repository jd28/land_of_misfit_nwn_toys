#pragma once

#include "../containers/Container.h"
#include <string>
#include <unordered_map>
#include <vector>

struct ResourceEntry {
    Container *container = nullptr;
    CKeyTableEntry *key = nullptr;
    ContainerElement element;
    bool dynamic = false;
    time_t last_demand = 0;
    char *cache = nullptr;

    ResourceEntry() = default;
    ResourceEntry(Container *cont, CKeyTableEntry *k,
                  ContainerElement ele, bool dynamic = false)
        : container(cont)
        , key(k)
        , element(ele)
        , dynamic{dynamic}
    {}

    // Validity is determined solely by whether container is not null.
    bool valid() const { return !!container; }
};

class ResourceLocator {
    using LookupTable = std::unordered_map<std::string, ResourceEntry>;

    static ResourceEntry INVALID_ENTRY;
    static std::string resref_cache;

    LookupTable resources;
    std::vector<Container*> dynamics;
    std::vector<Container*> statics;

public:
    // Add a dynamic container.  These are containers that can load resources from
    // any location.
    void AddDynamic(Container* cont);

    // Add a static container.  These are containers whose contents never change.
    // E,g. Erf, Zip, Key/Bif, etc.
    void AddStatic(Container* cont);

    // Only checks the lookup table, it does not query for updated resource.
    ResourceEntry& Get(const std::string &resref);

    bool HasContainer(const char *name) const;

    // Query for a ResourceEntry.  This will check all dynamic containers for the
    // most recently modified version of a resource.
    ResourceEntry& Query(const CResRef &resref, uint16_t type, CExoKeyTable **v1);

};
