#include "NWNXCombat.h"

extern CNWNXCombat combat;

int32_t Hook_GetRangedAttackBonus(CNWSCreatureStats *stats, int32_t use_base_eff, int32_t use_dex) {
    CNWSCreature *cre = stats ? stats->cs_original : NULL;
    if ( !cre ) { return 0; }

    auto c = combat.get_creature(cre->obj.obj_id);
    if ( !c ) { return 0; }

    return c->offense.getAttackBonus(0, use_base_eff, use_base_eff);
}
