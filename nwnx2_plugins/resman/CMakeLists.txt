add_module(
    resman
    HookDemandRes
    Hooks
    NWNXResMan
    plugin-resman
    pluginlink.h
    util.h

    resources/ResourceLocator
    resources/ResourceManager

    containers/Bif
    containers/Container
    containers/Directory
    containers/Erf
    containers/Key
    containers/ResmanDirectory
    containers/Zip
    containers/minizip/ioapi
    containers/minizip/mztools
    containers/minizip/unzip
)

target_link_libraries(resman z)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Wno-unused-parameter -Wno-unused -Wno-write-strings -Wno-return-type -std=c++11 -g -ggdb3")
