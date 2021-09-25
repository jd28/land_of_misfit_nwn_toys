#include "NWNXSolstice.h"

extern lua_State *L;
extern CNWNXSolstice solstice;

void Func_RemoveObjectFromCache(CGameObject *ob, char *value)
{
    if (!nl_pushfunction(L, "_SOL_REMOVE_CACHED_OBJECT"))
        return;

    lua_pushinteger(L, atoi(value));

    if (lua_pcall(L, 1, 0, 0) != 0) {
        solstice.Log(0, "ERROR: %s\n", lua_tostring(L, -1));
    }
}
