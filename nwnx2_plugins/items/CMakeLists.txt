include_directories(. ../../talib)

# Add the user's chosen flags to the build for this directory.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_module(items NWNXItems plugin-items
    handle_request
    hook_funcs
    hooks/h_CalculateBaseCosts
    hooks/h_CanEquipItem
    hooks/h_CanUnequipItem
    hooks/h_CanUseItem
    hooks/h_CheckProficiencies
    hooks/h_ComputeWeight
    hooks/h_GetIsHelmetHidden
    hooks/h_GetMinEquipLevel
    hooks/h_UpdateAppearanceForEquips

    local/l_GetIsHelmetHidden
    pluginlink.h
    ../../talib/nwn/all
)

target_link_libraries(items rt)
