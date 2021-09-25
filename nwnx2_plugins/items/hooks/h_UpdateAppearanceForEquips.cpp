/***************************************************************************
    NWNX Items
    (c) 2012-2013 jmd (jmd2028 at gmail dot com)

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

#include "NWNXItems.h"

uint32_t Hook_UpdateAppearanceForEquips(CNWSCreature *cre) {
    if(cre == NULL) { return OBJECT_INVALID; }
    
    CNWSItem *lefthand  = CNWSInventory__GetItemInSlot(cre->cre_equipment, EQUIPMENT_SLOT_LEFTHAND);
    CNWSItem *righthand = CNWSInventory__GetItemInSlot(cre->cre_equipment, EQUIPMENT_SLOT_RIGHTHAND);
    CNWSItem *cloak     = CNWSInventory__GetItemInSlot(cre->cre_equipment, EQUIPMENT_SLOT_CLOAK);
    CNWSItem *chest     = CNWSInventory__GetItemInSlot(cre->cre_equipment, EQUIPMENT_SLOT_CHEST);
    CNWSItem *head      = CNWSInventory__GetItemInSlot(cre->cre_equipment, EQUIPMENT_SLOT_HEAD);

    uint32_t id_left  = OBJECT_INVALID;
    uint32_t id_right = OBJECT_INVALID;
    uint32_t id_cloak = OBJECT_INVALID;
    uint32_t id_head  = OBJECT_INVALID;
    uint32_t id_chest = OBJECT_INVALID;
    uint8_t rh_vfx = 0;
    uint8_t lh_vfx = 0;

    if ( lefthand ) {
        id_left = lefthand->obj.obj_id;
        lh_vfx = lefthand->it_wpn_vfx;
    }

    if ( righthand ) {
        id_right = righthand->obj.obj_id;
        rh_vfx = righthand->it_wpn_vfx;
    }

    if ( cloak ) {
        id_cloak = cloak->obj.obj_id;
    }

    if ( chest ) {
        id_chest = chest->obj.obj_id;
    }

    if ( head ) {
        id_head = head->obj.obj_id;
    }

    if( cre->cre_appearance_info.cai_lefthand_obj_id !=  id_left   ||
        cre->cre_appearance_info.cai_righthand_obj_id !=  id_right ||
        cre->cre_appearance_info.cai_chest_obj_id !=  id_chest     ||
        cre->cre_appearance_info.cai_head_obj_id !=  id_head       ||
        cre->cre_appearance_info.cai_cloak_obj_id !=  id_cloak     ||
        cre->cre_appearance_info.cai_righthand_wpn_vfx != rh_vfx   ||
        cre->cre_appearance_info.cai_lefthand_wpn_vfx != lh_vfx ) {

        cre->cre_appearance_info.cai_lefthand_obj_id = id_left;
        cre->cre_appearance_info.cai_righthand_obj_id = id_right;
        cre->cre_appearance_info.cai_chest_obj_id = id_chest;
        
        if( Local_GetIsHelmetHidden(cre) ) {
            cre->cre_appearance_info.cai_head_obj_id = OBJECT_INVALID;
        }
        else {
            cre->cre_appearance_info.cai_head_obj_id = id_head;
        }
        
        cre->cre_appearance_info.cai_cloak_obj_id = id_cloak;
        cre->cre_appearance_info.cai_righthand_wpn_vfx = rh_vfx;
        cre->cre_appearance_info.cai_lefthand_wpn_vfx = lh_vfx;
    }
    
    return id_cloak;
}
