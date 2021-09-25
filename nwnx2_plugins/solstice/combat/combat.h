#ifndef _NWNX_COMBAT_COMBAT_H_
#define _NWNX_COMBAT_COMBAT_H_

#include "talib/util/timer.h"
#include "NWNXSolstice.h"

extern CNWNXSolstice solstice;

template <typename AttackType>
void ResolveMeleeAttack(CNWSCreature *attacker, CNWSObject *target,
                        int32_t attack_count, int32_t anim) {
    AttackType attack(attacker, target, false);

    attack.resolvePreAttack();

    for ( int i = 0; i < attack_count; ++i ) {
        attack.resolve();
        CNWSCreature__ResolveMeleeAnimations(attacker, i, attack_count, target, anim);
        attack.update();
    }

    CNWSCreature__SignalMeleeDamage(attacker, target, attack_count);
}

// called by nwnx
template <typename AttackType>
void ResolveRangedAttack(CNWSCreature *attacker, CNWSObject *target,
                         int32_t attack_count, int32_t anim) {

    AttackType attack(attacker, target, true);

    // CombatInfo pointers need to be updated before resolve ammo.
    attack.resolvePreAttack();

    // Attack count can be modified if, say, a creature only has less arrows left than attacks
    // or none at all.
    attack_count = attack.resolveAmmo(attack_count, false);

    if ( attack_count == 0 ) {
        attack.resolveNoAmmo();
        return;
    }

    for ( int i = 0; i < attack_count; ++i ) {
        attack.resolve();

        if ( !attack.isHit() ) {
            // This constantly throws arithmetic exceptions...
            // not sure what the issue is.
            //CNWSCreature__ResolveRangedMiss(attacker, target);
        }
        CNWSCreature__ResolveRangedAnimations(attacker, target, anim);

        attack.update();
    }

    CNWSCreature__SignalRangedDamage(attacker, target, attack_count);
    attack.resolveAmmo(attack_count, true);
}

#endif // _NWNX_COMBAT_COMBAT_H_
