#include "../NWNXResMan.h"
#include "NwnDefines.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <boost/algorithm/string.hpp>
#include "ResmanDirectory.h"

extern CNWNXResMan resman;

static char path[MAXPATH + 21];

time_t ResmanDirectoryContainer::GetLastModTime(const char *file)
{
    struct stat info;
    if (stat(file, &info) == 0) {
        return info.st_mtim.tv_sec;
    }
    return -1;
}

ContainerElement ResmanDirectoryContainer::get(size_t index, CKeyTableEntry **)
{
    // We don't know how to handle by index.
    return ContainerElement();
}

ContainerElement ResmanDirectoryContainer::get(const std::string &resref, CKeyTableEntry **)
{
    ContainerElement ce;
    const char *ext = strchr(resref.c_str(), '.');
    if (!ext) { return ce; }
    ++ext;
    snprintf(path, MAXPATH, "%s/%s/%s", path_.c_str(), ext, resref.c_str());
    struct stat info;
    if (stat(path, &info) == 0) {
        ce.mtime = info.st_mtim.tv_sec;
        ce.size = info.st_size;
        ce.internal = nullptr; // Nothing to pass back.
    } else {
        resman.Log(3, "o Skip - Unable to open file: %s\n", path);
    }
    return ce;
}

size_t ResmanDirectoryContainer::Demand(const std::string &resref, ContainerElement element, void *data)
{
    const char *ext = strchr(resref.c_str(), '.');
    if (!ext) { return 0; }
    ++ext;
    snprintf(path, MAXPATH, "%s/%s/%s", path_.c_str(), ext, resref.c_str());

    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        resman.Log(3, "o Skip - Unable to open file: %s\n", path);
        return 0;
    }
    size_t size = element.size;
    if (fread(data, size, 1, f) != 1) {
        resman.Log(3, "o Skip - Error reading file: %s\n", path);
        size = 0;
    }

    fclose(f);

    return size;
}

size_t ResmanDirectoryContainer::size() const
{
    return 0; // We don't support integer indexing.
}
