#include "Container.h"

class ResmanDirectoryContainer : public Container {
    std::string path_;
public:
    static time_t GetLastModTime(const char *file);

    ResmanDirectoryContainer(std::string path, const char* name)
        : Container(name)
        , path_{std::move(path)}
    {}
    virtual ContainerElement get(size_t index, CKeyTableEntry **) override;
    virtual ContainerElement get(const std::string &resref, CKeyTableEntry **) override;
    virtual size_t Demand(const std::string &resref, ContainerElement element, void *data) override;
    virtual size_t size() const override;
};
