#include "Container.h"

class DirectoryContainer : public Container {
    std::string path;
public:

    DirectoryContainer(std::string path, const char* name)
        : Container(name)
        , path{std::move(path)}
    {}
    virtual ContainerElement get(size_t index, CKeyTableEntry **) override;
    virtual ContainerElement get(const std::string &resref, CKeyTableEntry **) override;
    virtual size_t Demand(const std::string &resref, ContainerElement element, void *data) override;
    virtual size_t size() const override;
    ~DirectoryContainer() {}
};
