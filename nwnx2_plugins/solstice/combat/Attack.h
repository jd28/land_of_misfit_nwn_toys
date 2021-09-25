#ifndef NWNX_COMBAT_ATTACK_H
#define NWNX_COMBAT_ATTACK_H

#include "NWNXSolstice.h"

class Attack
{
public:
    Attack(CNWSCreature *attacker, CNWSObject *target, bool ranged);
    ~Attack() { }

    int32_t   getWeaponAttackType();
    // Public interface exposed in combat.h
    bool     isHit();
    bool     isSpecialAttack();

    uint32_t resolveAmmo(uint32_t attack_count,
                         bool force_equip = false);
    void     resolveNoAmmo();
    void     resolve();
    void     resolvePreAttack();
    void     update();

    CNWSCreature         *attacker_nwn;
    CNWSObject           *target_nwn;

    CNWSCombatAttackData *attack;
    int32_t               weapon;
    int32_t               ranged_type = 0;
    int32_t               target_state;
    int32_t               situational_flags;
    double                target_distance;
    bool                  is_offhand;
    bool                  is_sneak;
    bool                  is_death;
    bool                  is_killing;

    int32_t               damage_total;
    DamageResult          dmg_result;

    int32_t               effects_to_remove[DAMAGE_INDEX_NUM + 1];
    int32_t               effects_to_remove_len;
};

inline bool Attack::isHit()
{
    int r = attack->cad_attack_result;
    return r == 1 || r == 3 || r == 5 || r == 6 || r == 7 || r == 10;
}

inline bool Attack::isSpecialAttack()
{
    return !!attack->cad_special_attack;
}

extern "C" {
    Attack        *Local_GetAttack();
}

#endif // NWNX_COMBAT_ATTACK_H
