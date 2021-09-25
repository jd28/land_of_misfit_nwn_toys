/***************************************************************************
    Copyright (C) 2013 jmd (jmd2028 at gmail dot com)

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

#ifndef NWNX_ITEMS_HOOKS_H
#define NWNX_ITEMS_HOOKS_H

uint32_t Hook_CalculateBaseCosts (CNWSItem *item);
int Hook_CanEquipItem(CNWSCreature *cre, CNWSItem *item, uint32_t *d, int a, int b, int c, CNWSPlayer *pc);
int Hook_CanUnequipItem(CNWSCreature *cre, CNWSItem *item);
int32_t Hook_CheckProficiencies(CNWSCreature *cre, CNWSItem *item, int32_t a);
int32_t Hook_ComputeWeight(CNWSItem *);
int32_t Hook_CanUseItem(CNWSCreature *cre, CNWSItem *item, int32_t a);
void Hook_GetIsHelmetHidden1();
void Hook_GetIsHelmetHidden2();
int8_t Hook_GetMinEquipLevel (CNWSItem *item);
uint32_t Hook_UpdateAppearanceForEquips(CNWSCreature *cre);

#endif // NWNX_ITEMS_HOOKS_H
