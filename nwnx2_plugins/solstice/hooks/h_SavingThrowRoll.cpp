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

#include "NWNXCombat.h"

extern CNWNXCombat combat;

int32_t Hook_SavingThrowRoll(CNWSCreature *cre, uint8_t save_type, uint16_t dc, uint8_t save_vs_type, 
                             uint32_t versus_id, int32_t send_feedback, uint16_t feat, int32_t from_combat) {
    if(cre == NULL || cre->cre_stats == NULL)
        return 0;

    auto c = combat.get_creature(cre->obj.obj_id);
    if ( !c ) { return 0; }

    VersusInfo vs(nwn_GetCreatureByID(versus_id));
    SaveResult result = c->defense.doSavingThrow(save_type, dc, save_vs_type, vs, send_feedback);

    return result.first;
}
