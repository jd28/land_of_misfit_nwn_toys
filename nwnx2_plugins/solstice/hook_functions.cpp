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

#include "NWNXSolstice.h"
#include "hooks.h"

extern CNWNXSolstice solstice;
extern lua_State *L;

int (*CNWSModule__LoadModuleStartNext)(CNWSModule *mod, void *a2) = NULL;
int (*CNWSCreatureStats__GetSkillRank_orig)(CNWSCreatureStats *, uint8_t, CNWSObject *, int32_t) = NULL;
void (*CNWSCreature__ResolveRangedAttack_orig)(CNWSCreature *, CNWSObject *, int, int) = NULL;
void (*CNWSCreature__ResolveMeleeAttack_orig)(CNWSCreature *, CNWSObject *, int, int) = NULL;
void (*CNWSCreatureStats__UpdateCombatInformation_orig)(CNWSCreatureStats *) = NULL;
int (*CExoArrayList__CGameEffect_Add)(CExoArrayList_ptr *list, CGameEffect *eff) =
        reinterpret_cast<int (*)(CExoArrayList_ptr*, CGameEffect*)>(0x08061028);

bool hook_functions()
{
    NX_HOOK(CNWSCombatRound__AddAttackOfOpportunity, 0x080E31E0, Hook_AddAttackOfOpportunity, 5);

    nx_hook_function((void *)0x0815479c,
                     (void *)Hook_GetFeatTotalUses, 5, NX_HOOK_DIRECT);
    nx_hook_function((void *)0x08153E00,
                     (void *)Hook_GetFeatRemainingUses, 5, NX_HOOK_DIRECT);

    NX_HOOK(CNWSModule__LoadModuleStartNext,
         0x081B4A4C,
         Hook_LoadModuleStart, 5);

    nx_hook_function((void *)0x080F982C,
                     (void *)Hook_SetCombatMode, 5, NX_HOOK_DIRECT);

    NX_HOOK(CNWSCreature__ResolveMeleeAttack_orig, 0x080E9930, Hook_ResolveMeleeAttack, 5);
    NX_HOOK(CNWSCreature__ResolveRangedAttack_orig, 0x080E6FE4, Hook_ResolveRangedAttack, 5);

    NX_HOOK(CNWSCreatureStats__UpdateCombatInformation_orig,
         0x08142134,
         Hook_UpdateCombatInformation,
         5);

    NX_HOOK(CNWSCreature__UpdateAttributesOnEffect,
         0x0811E0A8,
         Hook_UpdateAttributesOnEffect,
         5);

    NX_HOOK(CNWSCreatureStats__GetEffectImmunity,
         0x0815FF10,
         Hook_GetEffectImmunity,
         5);

    NX_HOOK(CNWSCreature__GetTotalEffectBonus,
         0x08132298,
         Hook_GetTotalEffectBonus,
         5);

    NX_HOOK(CNWSCombatRound__InitializeNumberOfAttacks,
         0x080E2260,
         Hook_InitializeNumberOfAttacks,
         5);

    NX_HOOK(CNWSCreatureStats__GetWeaponFinesse,
         0x08155CF4,
         Hook_GetWeaponFinesse,
         5);


    NX_HOOK(CNWSCreatureStats__GetCriticalHitMultiplier,
         0x0814C4A0,
         Hook_GetCriticalHitMultiplier,
         5);

    NX_HOOK(CNWSCreatureStats__GetCriticalHitRoll,
         0x0814C31C,
         Hook_GetCriticalHitRoll,
         5);


    NX_HOOK(CNWSCreature__ResolveDamageShields,
         0x080EFCAC,
         Hook_ResolveDamageShields,
         5);

    NX_HOOK(CNWSObject__DoDamageImmunity, 0x081CDC4C, Hook_DoDamageImmunity, 5);
    NX_HOOK(CNWSObject__DoDamageReduction, 0x081CBD74, Hook_DoDamageReduction, 5);
    NX_HOOK(CNWSObject__DoDamageResistance, 0x081CC7BC, Hook_DoDamageResistance, 5);

    nx_hook_function((void*)0x081D65D8,
                     (void*)Hook_CExoArrayList_CGameEffect_Insert, 5, NX_HOOK_DIRECT);
    NX_HOOK(CNWSCreature__RemoveBadEffects, 0x0812DFB8,
         Hook_RemoveBadEffects, 5);

    /*
        nx_hook_function((void*)0x0812e19c,
                         (void*)Hook_GetArmorClass,
                         5, NX_HOOK_DIRECT);


        //HOOK(CNWSCreature__SavingThrowRoll, 0x080F0A90, Hook_SavingThrowRoll, 5);

        HOOK(CNWSCreatureStats__GetSkillRank_orig, 0x08152F5C, Hook_GetSkillRank, 5);

        nx_hook_function((void*)0x0814425c,
                         (void*)Hook_GetCreatureDamageDice,
                         5, NX_HOOK_DIRECT);

        nx_hook_function((void*)0x08144368,
                         (void*)Hook_GetCreatureDamageDie,
                         5, NX_HOOK_DIRECT);

        nx_hook_function((void*)0x08143068,
                         (void*)Hook_GetMeleeAttackBonus,
                         5, NX_HOOK_DIRECT);

        nx_hook_function((void*)0x08143524,
                         (void*)Hook_GetMeleeDamageBonus,
                         5, NX_HOOK_DIRECT);

        nx_hook_function((void*)0x0814374c,
                         (void*)Hook_GetRangedAttackBonus,
                         5, NX_HOOK_DIRECT);

        nx_hook_function((void*)0x08143b5c,
                         (void*)Hook_GetRangedDamageBonus,
                         5, NX_HOOK_DIRECT);

        nx_hook_function((void*)0x08143f88,
                         (void*)Hook_GetUnarmedDamageDice,
                         5, NX_HOOK_DIRECT);

        nx_hook_function((void*)0x0814408c,
                         (void*)Hook_GetUnarmedDamageDie,
                         5, NX_HOOK_DIRECT);

        nx_hook_function((void*)0x08209e94,
                         (void*)Hook_ExecuteCommandDoTouchAttack,
                         5, NX_HOOK_DIRECT);

    */

    return true;
}
