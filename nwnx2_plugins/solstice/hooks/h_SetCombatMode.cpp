/***************************************************************************
    Copyright (C) 2011-2012 jmd (jmd2028 at gmail dot com)

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

void Hook_SetCombatMode(CNWSCreature *cre, int8_t mode, int32_t change)
{
    if (!nl_pushfunction(L, "__SetCombatMode"))
        return;

    // Push object ID.
    lua_pushinteger(L, cre->obj.obj_id);
    lua_pushinteger(L, mode);
    lua_pushinteger(L, change);

    if (lua_pcall(L, 3, 0, 0) != 0) {
        solstice.Log(0, "SOLSTICE: __SetCombatMode : %s\n", lua_tostring(L, -1));
    }
}
