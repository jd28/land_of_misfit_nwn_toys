#include "NWNXSolstice.h"
#include "funcs.h"

extern CNWNXSolstice solstice;
extern lua_State *L;

char* HandleRequest(CGameObject *ob, const char *request, char *value)
{
    uint32_t obj = OBJECT_INVALID;

#define M(str, lit)                                                     \
    strncmp((str), "" lit, (sizeof (lit)/sizeof(char)) - 1) == 0

    if (M(request, "REMOVEFROMCACHE")) {
        Func_RemoveObjectFromCache(ob, value);
    } else if (M(request, "UPDATECOMBATINFO")) {
        if (!nl_pushfunction(L, "NWNXSolstice_UpdateCombatInfo"))
            return NULL;
        sscanf(value, "%x", &obj);
        lua_pushinteger(L, obj);

        if (lua_pcall(L, 1, 0, 0) != 0) {
            solstice.Log(0, "SOLSTICE: NWNXSolstice_UpdateCombatInfo : %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
    else if (M(request, "RUNSCRIPT")) {
        if (!nl_pushfunction(L, "_SOL_RUN_SCRIPT"))
            return 0;

        lua_pushstring(L, value);
        lua_pushinteger(L, ob->id);

        if (lua_pcall(L, 2, 2, 0) != 0) {
            solstice.Log(0, "ERROR: %s on %x %s\n", value, ob->id, lua_tostring(L, -1));
            lua_pop(L, 1);
            return 0;
        }

        int res = lua_tointeger(L, -1);
        int ret = -1;
        if (res != 0) {
            ret = lua_tointeger(L, -2);
        }
        lua_pop(L, 2);
        snprintf(value, strlen(value),"%d", ret);
    }
    else if (M(request, "RUNCOMMAND")) {
        if (!nl_pushfunction(L, "_RUN_COMMAND"))
            return NULL;

        lua_pushinteger(L, atoi(value));

        if (lua_pcall(L, 1, 0, 0) != 0) {
            solstice.Log(0, "Error: _RUN_COMMAND : %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
            return NULL;
        }
    }

    return NULL;
#undef M
}
