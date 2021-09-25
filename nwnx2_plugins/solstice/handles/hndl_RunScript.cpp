#include "NWNXSolstice.h"

extern lua_State *L;
extern CNWNXSolstice solstice;

int Handle_RunScript(uintptr_t p)
{
    CoreRunScriptEvent *s = reinterpret_cast<CoreRunScriptEvent *>(p);
    if (!s) { solstice.Log(0, "No core event!\n"); }

    if (!nl_pushfunction(L, "_SOL_RUN_SCRIPT"))
        return 0;

    (*NWN_VirtualMachine)->vm_object_id[(*NWN_VirtualMachine)->vm_recursion_level] = s->objectId;
    (*NWN_VirtualMachine)->vm_valid_obj[(*NWN_VirtualMachine)->vm_recursion_level] = s->recursionLevel;
    (*NWN_VirtualMachine)->vm_implementer->vmc_object_id = s->objectId;

    lua_pushstring(L, s->resref);
    lua_pushinteger(L, s->objectId);

    if (lua_pcall(L, 2, 2, 0) != 0) {
        solstice.Log(0, "ERROR: %s on %x %s\n",
                     s->resref, s->objectId,
                     lua_tostring(L, -1));
        lua_pop(L, 1);
        return 0;
    }

    int res = lua_tointeger(L, -1);
    if (res == 0) {
        solstice.lua_last_return = -1;
    } else {
        s->suppress = true;
        s->returnValue = lua_tointeger(L, -2);
    }
    lua_pop(L, 2);

    return res;
}

int Handle_RunScriptSituation(uintptr_t p)
{
    CoreRunScriptSituationEvent *s = reinterpret_cast<CoreRunScriptSituationEvent *>(p);

    if(!s || !s->marker) {
        return 0;
    }

    if (strcmp(s->marker, "$solstice") != 0) {
        return 0;
    }

    uint32_t temp = (*NWN_VirtualMachine)->vm_implementer->vmc_object_id;
    (*NWN_VirtualMachine)->vm_implementer->vmc_object_id = s->objectId;

    if (!nl_pushfunction(L, "_RUN_COMMAND"))
        return 0;

    lua_pushinteger(L, s->token);
    lua_pushinteger(L, s->objectId);

    if (lua_pcall(L, 2, 0, 0) != 0) {
        solstice.Log(0, "Error: _RUN_COMMAND : %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
        return 0;
    }

    s->suppress = 1;

    (*NWN_VirtualMachine)->vm_implementer->vmc_object_id = temp;

    return 1;
}
