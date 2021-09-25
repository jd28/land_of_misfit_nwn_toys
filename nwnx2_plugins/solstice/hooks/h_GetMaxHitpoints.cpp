/***************************************************************************
    Copyright (C) 2011-2013 jmd (jmd2028 at gmail dot com)

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
extern lua_State *L;

int Hook_GetMaxHitpoints(CNWSCreature  *cre, int32_t dunno)
{
    if (cre == NULL || cre->cre_stats == NULL)
        return 0;

    if (!nl_pushfunction(L, "__GetMaxHitpoints"))
        return 0;

    lua_pushinteger(L, cre->obj.obj_id);

    if (lua_pcall(L, 1, 1, 0) != 0) {
        solstice.Log(0, "SOLSTICE: __GetMaxHitpoints : %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
        return 0;
    }

    int result = lua_tointeger(L, -1);
    lua_pop(L, 1);

    solstice.Log(4, "Hook_GetMaxHitpoints: Creature: %x, Result: %d, Dunno: %d\n",
                 cre->obj.obj_id, result, dunno);

    return result;
}
