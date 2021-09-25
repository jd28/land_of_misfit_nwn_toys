/***************************************************************************
    Copyright (C) 2011 jmd ( jmd2028 at gmail dot com )

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ***************************************************************************/

#include "NWNXSolstice.h"

extern CNWNXSolstice solstice;

void lua_stackdump(lua_State* l)
{
    int i;
    int top = lua_gettop(l);

    solstice.Log(0, "total in stack %d\n", top);

    for (i = 1; i <= top; i++) {
        /* repeat for each level */
        int t = lua_type(l, i);
        switch (t) {
            case LUA_TSTRING:  /* strings */
                solstice.Log(0, "string: '%s'\n", lua_tostring(l, i));
                break;
            case LUA_TBOOLEAN:  /* booleans */
                solstice.Log(0, "boolean %s\n", lua_toboolean(l, i) ? "true" : "false");
                break;
            case LUA_TNUMBER:  /* numbers */
                solstice.Log(0, "number: %g\n", lua_tonumber(l, i));
                break;
            default:  /* other values */
                solstice.Log(0, "%s\n", lua_typename(l, t));
                break;
        }
        solstice.Log(0, "  ");  /* put a separator */
    }
    solstice.Log(0, "\n");  /* end the listing */
}

const char *lua_gettrace(lua_State *L)
{
    lua_getfield(L, LUA_GLOBALSINDEX, "debug");
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        return NULL;
    }
    lua_getfield(L, -1, "traceback");
    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 2);
        return NULL;
    }
    lua_pushvalue(L, 1);  /* pass error message */
    lua_pushinteger(L, 2);  /* skip this function and traceback */
    lua_call(L, 2, 1);  /* call debug.traceback */

    return lua_tostring(L, -1);
}

int nl_pushfunction(lua_State *L, const char* name)
{
    int res = 1;
    lua_getglobal(L, name);
    // Check for the functions existence.
    if (lua_isfunction(L, -1) == 0) {
        lua_pop(L, 1);
        solstice.Log(0, "%s not Defined\n", name);
        res = 0;
    }
    return res;
}

