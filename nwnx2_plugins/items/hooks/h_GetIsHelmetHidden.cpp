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

volatile CNWSCreature *cre;
volatile CNWSItem *item;

__attribute__((noinline))
int check(){
    return !Local_GetIsHelmetHidden((CNWSCreature *)cre)
        && cre->cre_equipment
        && (item = CNWSInventory__GetItemInSlot(cre->cre_equipment,
                                                EQUIPMENT_SLOT_HEAD));
}

void Hook_GetIsHelmetHidden1() {
    asm ("leave");
    asm ("movl 0xC(%ebp), %eax");
    asm ("movl %eax, cre");

    if(check()){
        asm("push $0x0806A678");
    }
    else{
        asm("push $0x0806A679");
    }
    asm("ret");
}

void Hook_GetIsHelmetHidden2() {
    asm ("leave");
    asm ("movl 0xC(%ebp), %eax");
    asm ("movl %eax, cre");

    if(check()){
        asm("mov item, %ebx");
        asm("push $0x0806A83E");
    }
    else{
        asm("push $0x0806A884");
    }
    asm("ret");
}    
