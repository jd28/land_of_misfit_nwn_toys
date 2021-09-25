set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.54 COMPONENTS system filesystem REQUIRED)

if (Boost_FOUND)


  # Add the user's chosen flags to the build for this directory.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-type -std=c++11")

  include_directories(
    .
    include
    external
    ${Boost_INCLUDE_DIR})

  set(NWCPP_HEADERS
  )

  add_module(cpp NWNXCpp plugin-nwcpp
    handles/hndl_RunScript
    handles/hndl_PluginsLoaded
    handles/hndl_RunScriptSituation
    include/ScriptLib
    include/ScriptManager
    include/builtins
    include/types
  )

  target_link_libraries(cpp
    ${Boost_LIBRARIES}
    rt)

else (Boost_FOUND)
        message(WARNING "Cannot find boost libraries, not building nwcpp")
endif (Boost_FOUND)
