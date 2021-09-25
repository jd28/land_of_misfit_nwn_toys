#include "NWNXSolstice.h"

extern CNWNXSolstice solstice;
extern lua_State *L;

int32_t Hook_GetArmorClass(CNWSCreature *cre)
{
    if (!nl_pushfunction(L, "__GetArmorClass"))
        return 0;

    // Push object ID.
    lua_pushinteger(L, cre->obj.obj_id);

    if (lua_pcall(L, 1, 1, 0) != 0) {
        solstice.Log(0, "SOLSTICE: __GetArmorClass : %s\n", lua_tostring(L, -1));
        return 0;
    }

    int32_t res = lua_tointeger(L, -1);
    lua_pop(L, 1);

    solstice.Log(3, "GetArmorClass: obj: %X, feat: %d, remaining: %d\n",
                 cre->obj.obj_id, res);

    return res;

}
