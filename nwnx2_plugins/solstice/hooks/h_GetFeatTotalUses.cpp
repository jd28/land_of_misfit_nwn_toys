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

int8_t Hook_GetFeatTotalUses(CNWSCreatureStats *stats, uint16_t feat)
{
    int8_t result = 0;
    int i, temp;
    CNWFeat *rule_feat;

    if (feat < 0 || feat >= (*NWN_Rules)->ru_feats_len) {
        return 0;
    } else {
        rule_feat = &(*NWN_Rules)->ru_feats[feat];
    }

    solstice.Log(3, "GetFeatTotalUses: Object: %x, Feat: %d\n",
                 stats->cs_original->obj.obj_id, feat);

    if (!CNWSCreatureStats__HasFeat(stats, feat)) {
        solstice.Log(3, "GetFeatTotalUses: Object: %x does not have feat: %d\n",
                     stats->cs_original->obj.obj_id, feat);
        return 0;
    }
    if (stats->cs_is_dm) { return 100; }

    if (!nl_pushfunction(L, "__GetMaximumFeatUses"))
        return 0;

    // Push object ID.
    lua_pushinteger(L, feat);
    lua_pushinteger(L, stats->cs_original->obj.obj_id);

    if (lua_pcall(L, 2, 1, 0) != 0) {
        solstice.Log(0, "SOLSTICE: __GetMaximumFeatUses : %s\n", lua_tostring(L, -1));
        return 0;
    }

    result = lua_tointeger(L, -1);
    lua_pop(L, 1);

    if (result > 100) {
        result = 100;
    } else if (result < 0) {
        result = 0;
    }

    solstice.Log(3, "GetFeatTotalUses: obj: %X, feat: %d, max uses: %d\n",
                 stats->cs_original->obj.obj_id,
                 feat,
                 result);

    return result;
}
