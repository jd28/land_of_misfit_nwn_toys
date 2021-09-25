#include "NWNXCombat.h"

extern CNWNXCombat combat;

uint8_t Hook_GetUnarmedDamageDice(CNWSCreatureStats *stats) {
    uint8_t res = 0;

    auto dmg = GetUnarmedBaseDamage(stats->cs_original, NULL);

    return dmg.roll.dice;
}
