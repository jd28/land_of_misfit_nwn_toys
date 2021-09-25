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

static void handle_effect(CNWSObject *obj, CGameEffect *eff, bool is_remove)
{
    if (!nl_pushfunction(L, "__HandleEffect"))
        return;

    solstice.last_effect.obj = obj;
    solstice.last_effect.eff = eff;
    solstice.last_effect.is_remove = is_remove;

    if (lua_pcall(L, 0, 0, 0) != 0) {
        solstice.Log(0, "SOLSTICE: __HandleEffect : %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    solstice.last_effect.obj = nullptr;
    solstice.last_effect.eff = nullptr;

}

// CNWSCreatureStats::GetEffectImmunity(uchar,CNWSCreature *) 0x0815FF10
int32_t Hook_GetEffectImmunity(CNWSCreatureStats *stats, uint8_t immunity,
                               CNWSCreature *vs)
{
    if (!nl_pushfunction(L, "__GetEffectImmunity"))
        return 0;

    // Push object ID.
    lua_pushinteger(L, stats->cs_original->obj.obj_id);
    lua_pushinteger(L, immunity);

    if (lua_pcall(L, 2, 1, 0) != 0) {
        solstice.Log(0, "SOLSTICE: __GetEffectImmunity : %s\n", lua_tostring(L, -1));
        return 0;
    }

    int32_t result = lua_toboolean(L, -1);
    lua_pop(L, 1);
    return result;
}

// CNWSCreature::GetTotalEffectBonus(uchar,CNWSObject *,int,int,uchar,uchar,uchar,uchar,int)
int32_t Hook_GetTotalEffectBonus(CNWSCreature *cre, uint8_t eff_switch ,
                                 CNWSObject *versus, int32_t elemental,
                                 int32_t is_crit, uint8_t save,
                                 uint8_t save_vs, uint8_t skill,
                                 uint8_t ability, int32_t is_offhand)
{

    int32_t args = 2;
    switch (eff_switch) {
        default:
            return CNWSCreature__GetTotalEffectBonus(cre, eff_switch , versus, elemental,
                    is_crit, save, save_vs, skill,
                    ability, is_offhand);
            break;
            // case 3:
            //     if(!nl_pushfunction(L, "__GetSaveEffectBonus"))
            //         return 0;
            //     args = 3;
            //     // Push object ID.
            //     lua_pushinteger(L, cre->obj.obj_id);
            //     lua_pushinteger(L, save);
            //     lua_pushinteger(L, save_vs);

            //     break;
        case 4:
            if (!nl_pushfunction(L, "__GetAbilityEffectBonus"))
                return 0;

            // Push object ID.
            lua_pushinteger(L, cre->obj.obj_id);
            lua_pushinteger(L, ability);

            break;
        case 5:
            if (!nl_pushfunction(L, "__GetSkillEffectBonus"))
                return 0;

            // Push object ID.
            lua_pushinteger(L, cre->obj.obj_id);
            lua_pushinteger(L, skill);

            break;
    }

    if (lua_pcall(L, args, 1, 0) != 0) {
        solstice.Log(0, "SOLSTICE: __GetTotalEffectBonus : %s\n", lua_tostring(L, -1));
        return 0;
    }

    int32_t result = lua_tointeger(L, -1);
    lua_pop(L, 1);
    return result;
}

void Hook_UpdateAttributesOnEffect(CNWSCreature *cre, CGameEffect *eff, int32_t a)
{
    handle_effect(&cre->obj, eff, false);
    CNWSCreature__UpdateAttributesOnEffect(cre, eff, a);
}
