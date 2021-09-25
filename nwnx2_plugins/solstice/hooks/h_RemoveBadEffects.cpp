/***************************************************************************
    Copyright (C) 2011-2013 jmd ( jmd2028 at gmail dot com )

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

void Hook_RemoveBadEffects(CNWSCreature *cre){
    CGameEffect *eff;
    uint16_t subtype = 0;
    uint16_t durtype = 0;
    int i = 0;

    while(i < cre->obj.obj_effects_len) {
        eff = cre->obj.obj_effects[i];
        subtype = nwn_GetSubType(eff);
        durtype = nwn_GetDurationType(eff);

        // When resting if an effect duration type is temporary or permenant and
        // is of the subtype magical or extraordinary, the effect is remove.
        if ( ( durtype == DURATION_TYPE_TEMPORARY ||
               durtype == DURATION_TYPE_PERMANENT ) &&
             ( subtype == SUBTYPE_MAGICAL ||
               subtype == SUBTYPE_EXTRAORDINARY)){
            CNWSObject__RemoveEffectById(&cre->obj, eff->eff_id);
        }
        else {
            ++i;
        }
    }
}
