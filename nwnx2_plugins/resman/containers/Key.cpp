#include "Key.h"
#include "../NWNXResMan.h"
#include <fstream>
#include <algorithm>
#include <tuple>

extern CNWNXResMan resman;

void Key::load() {
    std::ifstream file(fname.c_str(), std::ios::binary);
    if (!file.is_open()) { return; }

    KeyHeader header;
    file.seekg(0);
    file.read((char*)&header, sizeof(KeyHeader));

    std::vector<FileTable> fts;
    fts.resize(header.bif_count);
    file.seekg(header.offset_file_table);
    file.read((char*)&fts[0], sizeof(FileTable) * header.bif_count);

    bifs.reserve(header.bif_count);
    std::vector<std::string> bif_names;
    bif_names.reserve(header.bif_count);

    for ( auto it = fts.begin(); it != fts.end(); ++it ) {
        char buffer[256] = {0};

        file.seekg(it->name_offset);
        file.read(buffer, it->name_size -1);

        std::string s(buffer);
        std::replace( s.begin(), s.end(), '\\', '/');

        resman.Log(0, "Adding Bif: %s\n", s.c_str());
        bif_names.push_back(s);
        bifs.emplace_back(this, s);
    }

    file.seekg(header.offset_key_table);
    elements.resize(header.key_count);
    for ( size_t i = 0; i < header.key_count; ++i ) {
        elements[i].key.m_pRes = nullptr;
        elements[i].key.m_nRefCount = 0;

        file.read(elements[i].key.m_cResRef.m_resRef, 16);
        for(size_t i = 0; i < 16; ++i) {
            elements[i].key.m_cResRef.m_resRef[i] =
                    tolower(elements[i].key.m_cResRef.m_resRef[i]);
        }

        file.read((char*)&elements[i].key.m_nType, 2);
        file.read((char*)&elements[i].key.m_nID, 4);
    }

    std::sort(elements.begin(), elements.end(),
              [](const KeyTableElement& a,const KeyTableElement& b){
        uint16_t bif_idx_1 = a.key.m_nID >> 20;
        uint16_t bif_idx_2 = b.key.m_nID >> 20;
        uint32_t res_idx_1 = a.key.m_nID & 0xFFFFF;
        uint32_t res_idx_2 = b.key.m_nID & 0xFFFFF;
        return std::tie(bif_idx_1, res_idx_1) < std::tie(bif_idx_2, res_idx_2);
    });

/*
    resman.Log(0, "Dumping Key: %s\n", fname.c_str());
    for(auto& ele : elements) {
        uint16_t bif = ele.key.m_nID >> 20;
        uint32_t res = ele.key.m_nID & 0xFFFFF;
        if(bif < bifs.size() && res < bifs[bif].elements.size()){
            resman.Log(0, "    Resref: %.*s, Type: %d, Bif Index: %d, Res Index: %d, Size: %d\n",
                       16, ele.key.m_cResRef.m_resRef, ele.key.m_nType, bif, res,
                       bifs[bif].elements[res].size);
        }
        else {
            resman.Log(0, "    Invalid Index: Resref: %.*s, Type: %d, Bif Index: %d, Res Index: %d\n",
                       16, ele.key.m_cResRef.m_resRef, ele.key.m_nType, bif, res);
        }
            //, bifs[ele.key.m_nID >> 20].elements[ele.key.m_nID & 0xFFFFF].size);
    }
*/
}


Key::Key(std::string filename, const char *name)
    : Container(name)
    , fname(filename)
    , table(new CExoKeyTable)
{
    table->TableType = 1;
    load();
}

ContainerElement Key::get(size_t index, CKeyTableEntry **key)
{
    if(index >= elements.size()) { return ContainerElement(); }
    uint16_t bif_idx = elements[index].key.m_nID >> 20;
    uint32_t res_id = elements[index].key.m_nID & 0xFFFFF;
    if(bif_idx >= bifs.size() || res_id >= bifs[bif_idx].elements.size()) {
        return ContainerElement();
    }

    *key = &elements[index].key;
    return ContainerElement(bifs[bif_idx].elements[res_id].size, 0, &elements[index]);
}

ContainerElement Key::get(const std::string &resref, CKeyTableEntry **key)
{
    return ContainerElement();
}

size_t Key::Demand(const std::string &resref, ContainerElement element, void *data)
{
    KeyTableElement *e = (KeyTableElement *)element.internal;
    if(e) {
        uint16_t bif_idx = e->key.m_nID >> 20;
        uint32_t res_id = e->key.m_nID & 0xFFFFF;
        if(bif_idx < bifs.size()) {
            return bifs[bif_idx].Demand(res_id, data);
        }
    }
    return 0;
}

size_t Key::size() const
{
    return elements.size();
}
