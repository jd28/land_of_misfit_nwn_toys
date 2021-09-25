#include "NWNXCombat.h"

extern CNWNXCombat combat;

// Note that the touch attack parameter no longer does anything in this
// function.  See hooks/Hook_ExecuteCommandDoTouchAttack.cpp
int32_t Hook_GetMeleeAttackBonus(CNWSCreatureStats *stats, int offhand, int use_eff, int touch) {
    CNWSCreature *cre = stats ? stats->cs_original : NULL;
    if ( !cre ) { return 0; }

    auto c = combat.get_creature(cre->obj.obj_id);
    if ( !c ) { return 0; }

    uint32_t equip = offhand ? 1 : 0;

    if ( !c->offense.isEquipValid(equip) ) {
        equip = 2;
    }

    int32_t result = 0;
    
    if ( !touch ) {
        // The engine includes base attack bonus elsewhere in case
        // of non-touch attacks.
        result = c->offense.getAttackBonus(equip, false, !!use_eff);
    }

    return result;
}
