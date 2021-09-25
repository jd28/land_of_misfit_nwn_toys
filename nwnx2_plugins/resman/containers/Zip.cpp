#include "../NWNXResMan.h"
#include "Zip.h"
#include <cstring>
extern CNWNXResMan resman;

void ZipContainer::load()
{
    zf = unzOpen(path.c_str());
    resman.Log(0, "Zip file open: %s, : %d", path.c_str(), !!zf);
    char fname[64] = {0};
    int res = unzGoToFirstFile(zf);
    int i = 0;
    while(res == UNZ_OK) {
        ZipElement e;
        unz_file_info info;
        unzGetCurrentFileInfo(zf, &info, fname, 64, nullptr, 0, nullptr, 0);
        char *dot = strchr(fname, '.');
        if(dot && dot - fname < 16) {
            e.size = info.uncompressed_size;
            e.key.m_pRes = nullptr;
            e.key.m_nID = i;
            e.key.m_nRefCount = 0;
            strncpy(e.key.m_cResRef.m_resRef, fname, dot-fname);
            e.key.m_nType = NwnGetResTypeFromExtension(dot+1);
            elements.push_back(e);
            ++i;
        }
        res = unzGoToNextFile(zf);
    }
}


ContainerElement ZipContainer::get(size_t index, CKeyTableEntry **key)
{
    if(index >= elements.size()) {
        return ContainerElement();
    }
    ContainerElement e;
    e.mtime = 0;
    e.size = elements[index].size;
    e.internal = nullptr;
    *key = &elements[index].key;
    return e;
}

ContainerElement ZipContainer::get(const std::string &resref, CKeyTableEntry **)
{
    ContainerElement ce;
    if(unzLocateFile(zf, resref.c_str(), 0) == UNZ_OK) {
        unz_file_info info;
        unzGetCurrentFileInfo(zf, &info, nullptr, 0, nullptr, 0, nullptr, 0);
        ce.size = info.uncompressed_size;
        ce.internal = nullptr;
        ce.mtime = 0;
    }

    return ce;
}

size_t ZipContainer::Demand(const std::string &resref, ContainerElement element, void *data)
{
    size_t size = 0;
    if(unzLocateFile(zf, resref.c_str(), 0) == UNZ_OK) {
        unzOpenCurrentFile(zf);
        size = element.size;
        if(unzReadCurrentFile(zf, data, element.size) == 0) {
            // Not sure about the return here..
        }
        unzCloseCurrentFile(zf);
    }
    return size;
}

size_t ZipContainer::size() const
{
    return elements.size();
}
