#pragma once
#include "api/all.h"
#include "include/NwnDefines.h"
#include <time.h>
#include <string>

struct ContainerElement {
    ContainerElement () : size(0), internal(nullptr), mtime(0) {}
    ContainerElement (size_t size, time_t time, void* data = nullptr)
        : size(size)
        , internal(data)
        , mtime(time)
    {}

    // Validity of a container element is determined solely by the resource
    // size being greater than zero.
    bool valid() const { return size != 0; }

    // Size of the resource.
    size_t size;

    // Any internal data a container would like to store in the lookup table.
    // Note that this is never deallocated so whatever this points to really
    // needs to be stable.
    void *internal;

    // Last modification time.  For static containers 0 is a fine value.
    time_t mtime;
};

class Container;

class Container {
    std::string name_;

public:
    Container() = default;
    Container(const char *name) : name_{name} {}
    Container(const Container&) = delete;
    Container& operator=(const Container&) = delete;
    Container(Container&&) = default;
    Container& operator=(Container&&) = default;
    virtual ~Container() {}


    // Get an element by index.  Only static containers need to support this.
    // Obviously it doesn't make sense for all containers
    virtual ContainerElement get(size_t index, CKeyTableEntry **) = 0;

    // Really only dynamic containers need to implement this.  Since static containers
    // have their entire contents added to the lookup table on load.
    virtual ContainerElement get(const std::string& resref, CKeyTableEntry **) = 0;

    // Defaults to nullptr.
    virtual CExoKeyTable* GetKeyTable() { return nullptr; }

    // Gets the container name.
    const char* name() const { return name_.c_str(); }

    // Read resource data.  Note that data is has already been allocated.  The container merely
    // needs to read into it.
    virtual size_t Demand(const std::string& resref, ContainerElement element, void *data) = 0;

    // Returns the number of elements in the container.  Only needed for static containers.
    virtual size_t size() const = 0;
};
