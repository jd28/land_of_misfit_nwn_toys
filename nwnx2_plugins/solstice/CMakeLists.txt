find_path(LUAJIT_INCLUDE_DIR luajit.h
            NAMES luajit.h
            PATH_SUFFIXES luajit-2.0)

find_library(LUAJIT_LIBRARIES luajit NAMES libluajit-5.1.a)

# Add the user's chosen flags to the build for this directory.
set(CMAKE_CXX_FLAGS "-mtune=i386 -m32 -Wall -Wno-unused -Wno-write-strings -std=c++11 -O3")

if(LUAJIT_LIBRARIES)
include_directories(
    .
    ../../talib
    ${LUAJIT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR})

set(SOLSTICE_FUNCS_SRC
  funcs.h
  handle_request
  funcs/f_RemoveObjectFromCache
)

set(SOLSTICE_LUAFUNCS_SRC
  lua_funcs.h
  lua_funcs.cpp
)

add_module(solstice NWNXSolstice plugin-solstice
    hook_functions
    solstice_funcs
    ${SOLSTICE_LUAFUNCS_SRC}
    ${SOLSTICE_FUNCS_SRC}
    handles/hndl_RunScript
    handles/hndl_PluginsLoaded
    hooks/h_Effects
    hooks/h_Combat
    hooks/h_AddAttackOfOpportunity
    hooks/h_GetArmorClass
    hooks/h_GetFeatRemainingUses
    hooks/h_GetFeatTotalUses
    hooks/h_GetMaxHitpoints
    hooks/h_LoadModuleStart
    hooks/h_ResolveMeleeAttack
    hooks/h_ResolveRangedAttack
    hooks/h_SetCombatMode
    hooks/h_UpdateCombatInformation
    hooks/h_RemoveBadEffects
    hooks/h_CExoArrayList_CGameEffect_Insert
    local/l_NWNXLog
    local/l_SetDamageInfo
    local/l_SetCombatEngineActive
    local/l_GetLastEffect
    combat/Attack
    ../../talib/nwn/all
)
message(${CMAKE_MODULE_LINKER_FLAGS})
set(CMAKE_MODULE_LINKER_FLAGS ${CMAKE_MODULE_LINKER_FLAGS} -Wl,-E
CACHE STRING "Flags used by the linker during the creation.")

target_link_libraries(solstice
  rt
  libluajit-5.1.a)

endif(LUAJIT_LIBRARIES)
