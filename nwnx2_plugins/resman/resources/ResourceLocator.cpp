#include "../NWNXResMan.h"
#include "ResourceLocator.h"
#include "../util.h"
#include <algorithm>

extern CNWNXResMan resman;

std::string ResourceLocator::resref_cache{};

void ResourceLocator::AddStatic(Container *cont) {
    auto it = std::find_if(statics.cbegin(), statics.cend(),
                           [cont](const Container* c) -> bool {
        return strcmp(c->name(), cont->name()) == 0;
    });
    if(it != statics.cend()) {
        return;
    }
    statics.push_back(cont);
    resman.Log(0, "Adding static: %s\n", cont->name());
    for(size_t i = 0; i < cont->size(); ++i) {

        CKeyTableEntry *key = nullptr;
        ContainerElement e = cont->get(i, &key);
        if (!e.valid()) {
            continue;
        }
        resref_cache = resref_to_fname(key->m_cResRef, key->m_nType);
        auto entry = ResourceEntry{cont, key, e};
        auto it = resources.insert({resref_cache, entry});

        // If there is an entry in there already and it's not dynamic
        // just overwrite, precedence has already been determined.
        if(!it.second && !it.first->second.dynamic) {
            it.first->second = entry;
            resman.Log(4, "    Shadowing file in resource map: %s\n", resref_cache.c_str());
        }
        else {
            resman.Log(4, "    Adding file to resource map: %s\n", resref_cache.c_str());
        }
    }
}

ResourceEntry ResourceLocator::INVALID_ENTRY{};

void ResourceLocator::AddDynamic(Container *cont) {
    auto it = std::find_if(dynamics.cbegin(), dynamics.cend(),
                           [cont](const Container* c) -> bool {
        return strcmp(c->name(), cont->name()) == 0;
    });
    if(it != dynamics.cend()) {
        return;
    }
    dynamics.push_back(cont);
}

ResourceEntry& ResourceLocator::Get(const std::string &resref) {
    auto it = resources.find(resref);
    if(it != resources.end()) {
        return it->second;
    }
    return INVALID_ENTRY;
}

bool ResourceLocator::HasContainer(const char *name) const
{
    auto dit = std::find_if(dynamics.cbegin(), dynamics.cend(),
                           [name](const Container* c) -> bool {
        return strcasecmp(c->name(), name) == 0;
    });
    if(dit != dynamics.cend()) { return true; }

    auto sit = std::find_if(statics.cbegin(), statics.cend(),
                           [name](const Container* c) -> bool {
        return strcasecmp(c->name(), name) == 0;
    });
    if(sit != statics.cend()) { return true; }


    return false;
}

ResourceEntry& ResourceLocator::Query(const CResRef &resref, uint16_t type, CExoKeyTable **v1)
{
    resref_cache = resref_to_fname(resref, type);
    auto it = resources.find(resref_cache);
    ContainerElement dynres;
    Container *cont = nullptr;
    CKeyTableEntry *key = nullptr;
    bool insert = false;

    if(it != resources.end()) {
        dynres = it->second.element;
    }

    for(auto c : dynamics) {
        CKeyTableEntry *k = nullptr;
        ContainerElement t = c->get(resref_cache, &k);

        if(t.valid() && (!dynres.valid() || t.mtime > dynres.mtime)) {
            dynres = t;
            cont = c;
            key = k;
            insert = true;
        }
    }

    if(dynres.valid() && insert) {
        resman.Log(3, "  Inserting Dynamic Resource: %s\n", resref_cache.c_str());
        if(it == resources.end()) {
            auto it = resources.insert({resref_cache, ResourceEntry {
                                            cont,
                                            key ? key : resman.CreateKeyTableEntry(resref, (NwnResType)type),
                                            dynres,
                                            true
                                        }});
            return it.first->second;
        }
        else {
            it->second.container = cont;
            it->second.element = dynres;
            it->second.dynamic = true;
            return it->second;
        }
    }

    if(it != resources.end()) {
        return it->second;
    }
    return INVALID_ENTRY;
}
