#include "NWNXCombat.h"

extern CNWNXCombat combat;

int32_t Hook_GetMeleeDamageBonus(CNWSCreatureStats *stats, int32_t offhand, uint8_t a) {
    CNWSCreature *cre = stats ? stats->cs_original : NULL;
    if ( !cre ) { return 0; }

    auto c = combat.get_creature(cre->obj.obj_id);
    if ( !c ) { return 0; }

    uint32_t equip = offhand ? 1 : 0;

    if ( !c->offense.isEquipValid(equip) ) {
        equip = 2;
    }

    int32_t result = c->offense.getDamageBonus(equip);
    return result;
}
