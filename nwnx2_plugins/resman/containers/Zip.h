#include "Container.h"
#include "minizip/unzip.h"
#include <vector>

struct ZipElement {
    CKeyTableEntry key;
    size_t size;
};

class ZipContainer : public Container {
    std::string path;
    unzFile zf = NULL;
    size_t total_size = 0;
    std::vector<ZipElement> elements;

    void load();
public:
    ZipContainer(std::string path, const char* name)
        : Container(name)
        , path{std::move(path)}
    {
        load();
    }
    ~ZipContainer() {
        unzClose(zf);
    }

    virtual ContainerElement get(size_t index, CKeyTableEntry **key) override;
    virtual ContainerElement get(const std::string &resref, CKeyTableEntry **) override;
    virtual size_t Demand(const std::string &resref, ContainerElement element, void *data) override;
    virtual size_t size() const override;
};
