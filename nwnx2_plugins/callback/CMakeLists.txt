# Add the user's chosen flags to the build for this directory.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-type -std=c++11")

include_directories(.)

add_module(callback
   plugin-callback
   NWNXCallback
   handlers/hndl_PluginsLoaded
   handlers/hndl_RunScriptSituation)
