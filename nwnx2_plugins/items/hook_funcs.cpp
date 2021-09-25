#include "NWNXItems.h"
#include "hooks.h"

#define HOOK(orig, addr, hook, bytes)                                   \
    *(void**)&orig = nx_hook_function((void*)addr, (void*)hook, bytes, NX_HOOK_DIRECT | NX_HOOK_RETCODE)

extern CNWNXItems items;

bool hook_functions(){
    HOOK(CNWSCreature__CanEquipItem, 0x080FF978, Hook_CanEquipItem, 5);
    HOOK(CNWSCreature__CanUnEquipWeapon, 0x08107E78, Hook_CanUnequipItem, 5);
    HOOK(CNWSCreature__CanUseItem, 0x08110DF8, Hook_CanUseItem, 5);

    HOOK(CNWSItem__GetMinEquipLevel, 0x081A5DE4, Hook_GetMinEquipLevel, 5);
    HOOK(CNWSItem__CalculateBaseCosts, 0x081A5794, Hook_CalculateBaseCosts, 5);
    HOOK(CNWSItem__ComputeWeight, 0x081A137C, Hook_ComputeWeight, 5);

    if (items.override_proficiencies) {
        nx_hook_function((void *)0x080FFCD4,
                         (void *)Hook_CheckProficiencies,
                         5, NX_HOOK_DIRECT);
    }

    nx_hook_function((void *)0x0806a65e,
                     (void *)Hook_GetIsHelmetHidden1,
                     5, NX_HOOK_DIRECT);

    nx_hook_function((void *)0x0806a822,
                     (void *)Hook_GetIsHelmetHidden2,
                     5, NX_HOOK_DIRECT);

    nx_hook_function((void *)0x0812efa0,
                     (void *)Hook_UpdateAppearanceForEquips,
                     5, NX_HOOK_DIRECT);

    return true;
}
