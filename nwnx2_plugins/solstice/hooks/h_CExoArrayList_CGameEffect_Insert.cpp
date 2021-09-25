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
#include <tuple>
#include <utility>
#include "hooks.h"

extern CNWNXSolstice solstice;

static bool compare(int32_t type, CGameEffect *eff1, CGameEffect *eff2) {
    int subtype_loc = 0;
    
    switch(type) {
    default:
        return std::tie(eff1->eff_type, eff1->eff_creator, eff1->eff_spellid)
            <= std::tie(eff2->eff_type, eff2->eff_creator, eff2->eff_spellid);

    case EFFECT_TRUETYPE_DAMAGE_INCREASE:
    case EFFECT_TRUETYPE_DAMAGE_REDUCTION:
    case EFFECT_TRUETYPE_MISS_CHANCE:
    case EFFECT_TRUETYPE_SAVING_THROW_DECREASE:
    case EFFECT_TRUETYPE_SAVING_THROW_INCREASE:
        subtype_loc = 1;
        break;
    case EFFECT_TRUETYPE_ABILITY_DECREASE:
    case EFFECT_TRUETYPE_ABILITY_INCREASE:
    case EFFECT_TRUETYPE_AC_DECREASE:
    case EFFECT_TRUETYPE_AC_INCREASE:
    case EFFECT_TRUETYPE_DAMAGE_IMMUNITY_INCREASE:
    case EFFECT_TRUETYPE_DAMAGE_IMMUNITY_DECREASE:
    case EFFECT_TRUETYPE_DAMAGE_RESISTANCE:
    case EFFECT_TRUETYPE_IMMUNITY:
    case EFFECT_TRUETYPE_SKILL_DECREASE:
    case EFFECT_TRUETYPE_SKILL_INCREASE:
        subtype_loc = 0;
        break;

    case EFFECT_TRUETYPE_CONCEALMENT:
        subtype_loc = 4;
        break;	
    }
    
    return std::tie(eff1->eff_type, eff1->eff_integers[subtype_loc], eff1->eff_creator, eff1->eff_spellid)
        <= std::tie(eff2->eff_type, eff2->eff_integers[subtype_loc], eff2->eff_creator, eff2->eff_spellid);	
  
}

void Hook_CExoArrayList_CGameEffect_Insert(CExoArrayList_ptr *list,
                                           CGameEffect *eff,
                                           int loc) {
    int end = list->len;

    CExoArrayList__CGameEffect_Add(list, eff);
    
    // Shift back any effects
    while ( end > loc ) {
        --end;
        list->data[end+1] = list->data[end];
    }

    CGameEffect *e = reinterpret_cast<CGameEffect*>(list->data[loc - 1]);
    while ( loc > 0 && !compare(eff->eff_type, e, eff ) ) {
        list->data[loc] = list->data[loc - 1];
        --loc;
        e = reinterpret_cast<CGameEffect*>(list->data[loc - 1]);
    }

    list->data[loc] = eff;
}
