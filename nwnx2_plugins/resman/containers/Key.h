#pragma once
#include "Container.h"
#include "Bif.h"
#include <string>
#include <vector>
#include <memory>

struct KeyHeader {
    char     type[4];
    char     version[4];
    uint32_t bif_count;
    uint32_t key_count;
    uint32_t offset_file_table;
    uint32_t offset_key_table;
    uint32_t year;
    uint32_t day_of_year;
    char     reserved[32];
};

struct FileTable {
    uint32_t size;
    uint32_t name_offset;
    uint16_t name_size;
    uint16_t drives;
};

struct KeyTableElement {
    CKeyTableEntry key;
};


class Key : public Container
{
public:
    std::string fname;
    std::vector<Bif> bifs;
    std::vector<KeyTableElement> elements;
    std::unique_ptr<CExoKeyTable> table;
    void load();

    Key(std::string filename, const char* name);
    Key(const Key&) = delete;
    Key& operator=(const Key&) = delete;

    Key(Key&&) = default;
    Key& operator=(Key&&) = default;

    virtual ContainerElement get(size_t index, CKeyTableEntry **key) override;
    virtual ContainerElement get(const std::string& resref, CKeyTableEntry **key) override;
    virtual size_t Demand(const std::string& resref, ContainerElement element, void *data) override;
    virtual size_t size() const override;
    virtual CExoKeyTable* GetKeyTable() override { return table.get(); }
};
