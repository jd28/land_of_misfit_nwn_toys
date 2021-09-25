#include "../NWNXResMan.h"
#include "Directory.h"
#include "NwnDefines.h"

#include <sys/stat.h>
#include <cstring>

extern CNWNXResMan resman;

static char buffer[MAXPATH + 21];

static time_t GetLastModTime(const char *file)
{
    struct stat info;
    if (stat(file, &info) == 0) {
        return info.st_mtim.tv_sec;
    }
    return -1;
}

ContainerElement DirectoryContainer::get(size_t index, CKeyTableEntry **)
{
    // We don't know how to handle by index.
    return ContainerElement();
}

ContainerElement DirectoryContainer::get(const std::string &resref, CKeyTableEntry **)
{
    ContainerElement ce;
    snprintf(buffer, MAXPATH, "%s%s", path.c_str(), resref.c_str());
    struct stat info;
    if (stat(buffer, &info) == 0) {
        ce.mtime = info.st_mtim.tv_sec;
        ce.size = info.st_size;
        ce.internal = nullptr; // Nothing to pass back.
    } else {
        resman.Log(3, "o Skip - Unable to open file: %s\n", buffer);
    }
    return ce;
}

size_t DirectoryContainer::Demand(const std::string &resref, ContainerElement element, void *data)
{
    snprintf(buffer, MAXPATH, "%s%s", path.c_str(), resref.c_str());

    FILE *f = fopen(buffer, "rb");
    if (f == NULL) {
        resman.Log(3, "o Skip - Unable to open file: %s\n", buffer);
        return 0;
    }
    size_t size = element.size;
    if (fread(data, size, 1, f) != 1) {
        resman.Log(3, "o Skip - Error reading file: %s\n", buffer);
        size = 0;
    }

    fclose(f);

    return size;
}

size_t DirectoryContainer::size() const
{
    return 0; // We don't support integer indexing.
}
