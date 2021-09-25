#pragma once
#include <string>
#include <fstream>
#include <vector>

class Key;

struct BifEntry {
    uint32_t res_id;
    uint32_t offset;
    uint32_t size;
    uint32_t res_type;
};

struct BifHeader {
    char     type[4];
    char     version[4];
    uint32_t var_res_count;
    uint32_t fix_res_count;
    uint32_t var_table_offset;
};

struct BifElement {
    std::string filename;
    uint32_t    offset;
    uint32_t    size;

    bool operator <(const BifElement& other) const {
        return filename < other.filename;
    }
};

class Bif {
public:
    Key *key = nullptr;
    std::string fname;
    FILE *file = nullptr;
    std::vector<BifEntry> elements;

    void load();

    Bif(Key *key, std::string filename);
    Bif(const Bif&) = delete;
    Bif& operator=(const Bif&) = delete;

    Bif(Bif&& other)
        : key(other.key)
        , fname(std::move(other.fname))
        , file(other.file)
        , elements(std::move(other.elements)) {
        other.file = nullptr;
    }

    Bif& operator=(Bif&& other) {
        if(this != &other) {
            std::swap(key, other.key);
            std::swap(fname, other.fname);
            if(file) {
                fclose(file);
                file = nullptr;
            }
            std::swap(file, other.file);
            std::swap(elements, other.elements);
        }
        return *this;
    }

    ~Bif() {
        if(file)
            fclose(file);
    }

    size_t Demand(size_t index, void* data);
    size_t GetElementSize(size_t index);
};
