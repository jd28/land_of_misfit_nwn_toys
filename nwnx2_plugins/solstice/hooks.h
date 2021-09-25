#pragma once

int8_t  Hook_GetFeatRemainingUses(CNWSCreatureStats *stats, uint16_t feat);
int8_t  Hook_GetFeatTotalUses(CNWSCreatureStats *stats, uint16_t feat);
int     Hook_GetMaxHitpoints (CNWSCreature  *cre, int32_t dunno);
int     Hook_GetRunScriptReturnValue(CVirtualMachine *vm, int* a, void** b);
void    Hook_SetCombatMode(CNWSCreature *cre, int8_t mode, int32_t change);
void    Hook_AddAttackOfOpportunity(CNWSCombatRound *cr, uint32_t target);
int32_t Hook_LoadModuleStart(CNWSModule *mod, void *a2);
void    Hook_ResolveRangedAttack(CNWSCreature *attacker, CNWSObject *obj, int attack_count, int a);
void    Hook_ResolveMeleeAttack(CNWSCreature *attacker, CNWSObject *obj, int attack_count, int a);
void    Hook_UpdateCombatInformation(CNWSCreatureStats *cre);

extern int (*CNWSModule__LoadModuleStartNext)(CNWSModule *mod, void *a2);
extern int (*CNWSCreatureStats__GetSkillRank_orig)(CNWSCreatureStats *, uint8_t, CNWSObject *, int32_t);
extern void (*CNWSCreature__ResolveRangedAttack_orig)(CNWSCreature *, CNWSObject *, int, int);
extern void (*CNWSCreature__ResolveMeleeAttack_orig)(CNWSCreature *, CNWSObject *, int, int);
extern void (*CNWSCreatureStats__UpdateCombatInformation_orig)(CNWSCreatureStats *);
extern int (*CExoArrayList__CGameEffect_Add)(CExoArrayList_ptr *list, CGameEffect *eff);

int32_t Hook_GetArmorClass(CNWSCreature *cre);
void Hook_RemoveBadEffects(CNWSCreature *cre);
void Hook_CExoArrayList_CGameEffect_Insert(CExoArrayList_ptr *list, CGameEffect *eff, int location);

int32_t Hook_GetEffectImmunity(CNWSCreatureStats *stats, uint8_t immunity,
                               CNWSCreature *vs);
int32_t Hook_GetTotalEffectBonus(CNWSCreature *cre, uint8_t eff_switch ,
                                 CNWSObject *versus, int32_t elemental,
                                 int32_t is_crit, uint8_t save,
                                 uint8_t save_vs, uint8_t skill,
                                 uint8_t ability, int32_t is_offhand);
void Hook_InitializeNumberOfAttacks(CNWSCombatRound *cr);
int32_t Hook_GetWeaponFinesse(CNWSCreatureStats *stats, CNWSItem *item);
int32_t Hook_GetCriticalHitMultiplier(CNWSCreatureStats *stats, int32_t is_offhand);
int32_t Hook_GetCriticalHitRoll(CNWSCreatureStats *stats, int32_t is_offhand);
int32_t Hook_ResolveDamageShields(CNWSCreature *cre, CNWSCreature *attacker);

int32_t Hook_DoDamageImmunity(CNWSObject *obj, CNWSCreature *vs, int32_t amount,
                              uint16_t flags, int32_t no_feedback, int32_t from_attack);
int32_t Hook_DoDamageReduction(CNWSObject *obj, CNWSCreature *vs, int32_t amount,
                               uint8_t power, int32_t no_feedback, int32_t from_attack);
int32_t Hook_DoDamageResistance(CNWSObject *obj, CNWSCreature *vs, int32_t amount,
                                uint16_t flags, int32_t no_feedback, int32_t from_attack,
                                int32_t a);
void Hook_UpdateAttributesOnEffect(CNWSCreature *cre, CGameEffect *eff, int32_t a);
