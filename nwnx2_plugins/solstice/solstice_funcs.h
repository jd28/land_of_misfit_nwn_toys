#ifndef _NWNX_SOLSTICE_FUNCS_H_
#define _NWNX_SOLSTICE_FUNCS_H_

#include "NWNXSolstice.h"

#define ENGINE_NUM_STRUCTURES           5
#define ENGINE_STRUCTURE_EFFECT         0
#define ENGINE_STRUCTURE_EVENT          1
#define ENGINE_STRUCTURE_LOCATION       2
#define ENGINE_STRUCTURE_TALENT         3
#define ENGINE_STRUCTURE_ITEMPROPERTY   4

extern "C" {
    int  ns_BitScanFFS(uint32_t mask);
    void ns_DelayCommand(uint32_t objid, float delay, uint32_t token);
    void ns_ActionDoCommand(CNWSObject * obj, uint32_t token);

    const char** str_split(const char* s, const char* sep, bool isany);
    const char* str_rtrim(const char* str);
    const char* str_ltrim(const char* str);
    const char* str_trim(const char* str);

    void ns_AddOnHitSpells(CNWSCombatAttackData *data,
                           CNWSCreature *attacker,
                           CNWSObject *target,
                           CNWSItem *item,
                           bool from_target);

    uint32_t ns_GetAmmunitionAvailable(CNWSCreature *attacker, int32_t num_attacks, int32_t ranged_type, bool equip);
    void ns_PostPolymorph(CNWSCreature *cre, int32_t ignore_pos, bool is_apply);

    const char* ns_GetCombatDamageString(
        const char *attacker,
        const char *target,
        const DamageResult *dmg,
        bool simple);

    int ns_SendMessagePopupToPlayer(CNWSCreature *to, CNWSPlaceable *from, const char* message);
}
#endif
