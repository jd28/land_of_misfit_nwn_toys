/***************************************************************************
    Copyright (C) 2011-2012 jmd ( jmd2028 at gmail dot com )

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

extern CNWNXItems items;

// This is techincally CanUnequipWeapon, but all items run through it...
// In the case of this hook, it is a BAD idea to completely override this.
// So only if the event result is unequippable, can the original function
// be ignored.

int Hook_CanUnequipItem(CNWSCreature *cre, CNWSItem *item) {
    int ret = 1;
    if(cre && item) {
        items.FireEvent(ITEMS_EVENT_CAN_UNEQUIP, cre->obj.obj_id, item->obj.obj_id);
        if (items.event.use_result && items.event.result == 0)
            return 0;
    }

    return CNWSCreature__CanUnEquipWeapon(cre, item);
}
