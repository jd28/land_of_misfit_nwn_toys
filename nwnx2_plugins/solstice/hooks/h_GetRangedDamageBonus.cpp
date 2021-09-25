#include "NWNXCombat.h"

extern CNWNXCombat combat;

int32_t Hook_GetRangedDamageBonus(CNWSCreatureStats *stats) {
    int32_t res = 0;
    CNWSCreature *cre = stats->cs_original;
    auto c = combat.get_creature(cre->obj.obj_id);
    if ( c ) {
        res = c->offense.getDamageBonus(0);
    }

    return res;
}
