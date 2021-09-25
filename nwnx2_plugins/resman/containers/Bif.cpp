#include "Bif.h"
#include <stdint.h>

Bif::Bif(Key *key, std::string filename)
    : key(key), fname(filename)
{
    file = fopen(filename.c_str(), "rb");
    load();
}

size_t Bif::GetElementSize(size_t index)
{
    if(index < elements.size()) {
        return elements[index].size;
    }
    return 0;
}

void Bif::load()
{
    if ( !file ) { return; }
    BifHeader header;
    fread((char*)&header, sizeof(BifHeader), 1, file);

    uint32_t offset = header.var_table_offset;
    fseek(file, offset, SEEK_SET);
    elements.resize(header.var_res_count);
    fread(reinterpret_cast<char*>(&elements[0]), sizeof(BifEntry) * header.var_res_count, 1, file);

    for (auto& e : elements) {
        e.res_id &= 0xFFFF;
    }
}

size_t Bif::Demand(size_t index, void *data)
{
    if(index < elements.size()) {
        fseek(file, elements[index].offset, SEEK_SET);
        fread(data, elements[index].size, 1, file);
        return elements[index].size;
    }
    return 0;
}
