#include "NWNXSolstice.h"
#include "talib/util/timer.h"
#include "hooks.h"

extern CNWNXSolstice solstice;
extern lua_State *L;

void Hook_UpdateCombatInformation(CNWSCreatureStats *stats)
{
    // Run our stuff first...
    ++solstice.updates;
    auto start = ClockGetTime();
    if (!nl_pushfunction(L, "__UpdateCombatInfo"))
        return;

    lua_pushinteger(L, stats->cs_original->obj.obj_id);

    if (lua_pcall(L, 1, 0, 0) != 0) {
        solstice.Log(0, "SOLSTICE: __UpdateCombatInfo : %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
    solstice.update_time += ClockGetTime() - start;
    CNWSCreatureStats__UpdateCombatInformation_orig(stats);
}
