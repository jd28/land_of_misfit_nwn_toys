#include "Erf.h"
#include <algorithm>
#include <cstring>

void Erf::load()
{
    ErfHeader header;
    file.open(fname, std::ios::in | std::ios::binary);
    file.read((char*)&header, sizeof(ErfHeader));
    if( strncmp(header.type, "MOD", 3) != 0
            && strncmp(header.type, "HAK", 3) != 0
            && strncmp(header.type, "ERF", 3) != 0
            && strncmp(header.type, "SAV", 3) != 0) {
        return;
    }

    elements.resize(header.entry_count);

    file.seekg(header.offset_keys);
    for (size_t i = 0; i < header.entry_count; ++ i) {
        elements[i].key.m_nID = i;
        elements[i].key.m_pRes = nullptr;
        elements[i].key.m_nRefCount = 0;
        file.read(elements[i].key.m_cResRef.m_resRef, 16);
        for(size_t j = 0; j < 16; ++j) {
            elements[i].key.m_cResRef.m_resRef[j] =
                    tolower(elements[i].key.m_cResRef.m_resRef[j]);
        }
        file.seekg(4, std::ios_base::cur);
        file.read((char*)&elements[i].key.m_nType, 2);
        file.seekg(2, std::ios_base::cur);
    }

    file.seekg(header.offset_res);
    for (size_t i = 0; i < header.entry_count; ++ i) {
        file.read((char*)&elements[i].info, sizeof(ErfElementInfo));

    }
}

Erf::Erf(std::string filename, const char *name)
    : Container(name)
    , fname(filename)
    , table(new CExoKeyTable)
{
    table->TableType = 3;
    load();
}

size_t Erf::Demand(const std::string &resref, ContainerElement element, void *data)
{
    ErfElement& e = *(ErfElement*)element.internal;
    file.seekg(e.info.offset, std::ios_base::beg);
    file.read((char*)data, e.info.size);
    return e.info.size;
}

ContainerElement Erf::get(size_t index, CKeyTableEntry **key)
{
    if (index >= elements.size()) { return ContainerElement(); }
    *key = &elements[index].key;
    return ContainerElement(elements[index].info.size, 0, &elements[index]);
}

ContainerElement Erf::get(const std::string &resref, CKeyTableEntry **key)
{
    return ContainerElement();
}

size_t Erf::size() const { return elements.size(); }
