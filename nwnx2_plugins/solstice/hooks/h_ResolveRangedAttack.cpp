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
#include "hooks.h"
#include "talib/util/timer.h"
#include "combat/combat.h"
#include "combat/Attack.h"

extern CNWNXSolstice solstice;
extern lua_State *L;

void Hook_ResolveRangedAttack(CNWSCreature *attacker, CNWSObject *obj, int attack_count, int a)
{
    if (solstice.combat_engine_active) {
        lua_gc(L, LUA_GCSTOP, 0);
        solstice.lua_attacks += attack_count;
        auto start = ClockGetTime();
        ResolveRangedAttack<Attack>(attacker, obj, attack_count, a);
        solstice.lua_time += ClockGetTime() - start;
        lua_gc(L, LUA_GCRESTART, 0);
    } else {
        solstice.nwn_attacks += attack_count;
        auto start = ClockGetTime();
        CNWSCreature__ResolveRangedAttack_orig(attacker, obj, attack_count, a);
        solstice.nwn_time += ClockGetTime() - start;
    }
}
