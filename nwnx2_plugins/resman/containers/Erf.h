#pragma once

#include "Container.h"
#include <string>
#include <vector>
#include <fstream>
#include <memory>

struct ErfHeader {
    char type[4];
    char version[4];
    uint32_t locstring_count;
    uint32_t locstring_size;
    uint32_t entry_count;
    uint32_t offset_locstring;
    uint32_t offset_keys;
    uint32_t offset_res;
    uint32_t year;
    uint32_t day_of_year;
    uint32_t desc_strref;
    char reserved[116];
};

struct ErfKey {
    char resref[16];
    uint32_t res_id;
    uint16_t res_type;
    int16_t unused;
};

struct ErfElementInfo {
    uint32_t offset;
    uint32_t size;
};


struct ErfElement {
    CKeyTableEntry key;
    ErfElementInfo info;
};

class Erf : public Container {
public:
    std::vector<ErfElement> elements;
    std::fstream file;
    std::string fname;
    std::unique_ptr<CExoKeyTable> table;
    void load();
    Erf(std::string filename, const char* name);
    Erf(const Erf&) = delete;
    Erf& operator=(const Erf&) = delete;
    Erf(Erf&& other)
        : elements(std::move(other.elements))
        , file(other.fname, std::ios::in | std::ios::binary)
        , fname(std::move(other.fname))
        , table(std::move(other.table))
    {
    }
    Erf& operator=(Erf&&) = default;

    ~Erf() {}
    virtual size_t Demand(const std::string& resref, ContainerElement element, void *data) override;
    virtual ContainerElement get(size_t index, CKeyTableEntry **) override;
    virtual ContainerElement get(const std::string& resref, CKeyTableEntry **) override;
    virtual size_t size() const override;
    virtual CExoKeyTable* GetKeyTable() override { return table.get(); }
};
