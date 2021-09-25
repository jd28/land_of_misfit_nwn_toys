/***************************************************************************
    Copyright (C) 2011-2012 jmd (jmd2028 at gmail dot com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
***************************************************************************/

#include "NWNXCombat.h"

extern CNWNXCombat combat;
unsigned char ***pESP;

static bool CheckAddress(unsigned long addr) {
#define R(start, stop) if(addr >= start && addr <= stop)

    R(0x08212A5C, 0x08212B6B) return true; // "CNWVirtualMachineCommands::ExecuteCommandGetSkillRank"
    R(0x082286D4, 0x08228B0F) return true; // "CNWVirtualMachineCommands::ExecuteCommandGetIsSkillSuccessful"
    R(0x082143EC, 0x082146AC) return true; // "CNWVirtualMachineCommands::ExecuteCommandGetIsDoorActionPossible"
    R(0x08053054, 0x0805376B) return false; // "CNWSPlayerCharSheetGUI::ComputeCharacterSheetUpdateRequired");
    R(0x080580BC, 0x0805D90F) return false; //  "CNWSPlayer::ValidateCharacter");
    R(0x081D1CD4, 0x081D1FEB) return true; // "CNWSObject::BroadcastCounterSpellData");
    R(0x081C53D0, 0x081C6397) return true; // "CNWSObject::AIActionUnlockObject");
    R(0x081C6398, 0x081C7273) return true; // "CNWSObject::AIActionLockObject");
    R(0x0806DB28, 0x0806FBEB) return false; // "CNWSMessage::WriteGameObjUpdate_PlayerUpdate");
    R(0x08074DB8, 0x0807595F) return false; // "CNWSMessage::WriteGameObjUpdate_CharacterSheet");
    R(0x080747D0, 0x08074C4F) return false; //"CNWSMessage::SendServerToPlayerExamineGui_TrapData");
    R(0x081AE74C, 0x081AF1A3) return true; // "CNWSItemPropertyHandler::ApplyHolyAvenger");
    R(0x0816C7E4, 0x0816DC57) return true; // "CNWSEffectListHandler::OnApplyDamage");
    R(0x081579CC, 0x08158350) return false; // "CNWSCreatureStats::ValidateLevelUp");
    R(0x0814A47C, 0x0814A8EF) return false; // "CNWSCreatureStats::UpdateLastStatsObject");
    R(0x08150964, 0x08151713) return false; // "CNWSCreatureStats::SaveClassInfo");
    R(0x0814C578, 0x0814F8DB) return false; // "CNWSCreatureStats::ReadStatsFromGff");
    R(0x0815A0C4, 0x0815BC7F) return false; // "CNWSCreatureStats::LevelUpAutomatic");
    R(0x081590F0, 0x0815986E) return false; // "CNWSCreatureStats::LevelUp");
    R(0x08159870, 0x0815A0C3) return false; // "CNWSCreatureStats::LevelDown");
    R(0x081656D8, 0x08165719) return true; // "CNWSCreatureStats::GetTotalACSkillMod");
    R(0x08158748, 0x0815904D) return false; // "CNWSCreatureStats::GetMeetsPrestigeClassRequirements");
    R(0x0815FB84, 0x0815FD22) return true; // "CNWSCreatureStats::GetCreatureTalentRandomSkill");
    R(0x08155888, 0x08155B15) return true; // "CNWSCreatureStats::GetCreatureHasTalent");
    R(0x08161918, 0x08161AB3) return false; // "CNWSCreatureStats::GetCanUseSkillAfterLevelUp");
    R(0x08152E1C, 0x08152F5A) return false; // "CNWSCreatureStats::GetCanUseSkill");
    R(0x0814088C, 0x08141CB9) return true; // "CNWSCreatureStats::GetArmorClassVersus");
    R(0x08161160, 0x08161914) return false; // "CNWSCreatureStats::FeatRequirementsMetAfterLevelUp");
    R(0x0815BF44, 0x0815C533) return true; // "CNWSCreatureStats::FeatRequirementsMet");
    R(0x08131680, 0x0813179F) return true; // "CNWSCreature::UseLoreOnItem");
    R(0x0811A2F4, 0x0811AF4A) return true; // "CNWSCreature::UpdateTrapCheck");
    R(0x080F0A90, 0x080F182F) return true; // "CNWSCreature::SavingThrowRoll");
    R(0x080E71D4, 0x080E762F) return true; // "CNWSCreature::ResolveRangedSpecialAttack");
    R(0x080E9A98, 0x080EAA17) return true; // "CNWSCreature::ResolveMeleeSpecialAttack");
    R(0x080EB190, 0x080EB71F) return true; // "CNWSCreature::ResolveAttackRoll");
    R(0x081265C4, 0x08126B56) return true; // "CNWSCreature::DoSpotDetection");
    R(0x08125DDC, 0x081265C2) return true; // "CNWSCreature::DoListenDetection");
    R(0x0811172C, 0x0811192F) return true; // "CNWSCreature::CheckUseMagicDeviceSkill");
    R(0x08127CEC, 0x08128863) return true; // "CNWSCreature::BroadcastSpellCast");
    R(0x08128EA8, 0x081298AF) return true; // "CNWSCreature::BroadcastAttackOfOpportunity");
    R(0x08109BEC, 0x0810A23B) return true; // "CNWSCreature::AIActionTaunt");
    R(0x080F8174, 0x080F93D7) return true; // "CNWSCreature::AIActionSetTrap");
    R(0x080F63A8, 0x080F706F) return true; // "CNWSCreature::AIActionRecoverTrap");
    R(0x0810A3F4, 0x0810B3F7) return true; // "CNWSCreature::AIActionPickPocket");
    R(0x0810B3F8, 0x0810BDDB) return true; // "CNWSCreature::AIActionHeal");
    R(0x080F5D3C, 0x080F63A7) return true; // "CNWSCreature::AIActionFlagTrap");
    R(0x080F7B78, 0x080F8173) return true; // "CNWSCreature::AIActionExamineTrap");
    R(0x080F7070, 0x080F7B77) return true; // "CNWSCreature::AIActionDisarmTrap");
    R(0x081E16B8, 0x081E1957) return true; // "CNWSCreature::AIActionCastSpell");
    R(0x08108AFC, 0x08109113) return true; // "CNWSCreature::AIActionAnimalEmpathy");

    return false;

#undef R
}

static const char * GetFunctionCallerName(unsigned long addr) {
#define CNAME(start, stop, func_name) if(addr >= start && addr <= stop) return func_name
    
    CNAME(0x08212A5C, 0x08212B6B, "CNWVirtualMachineCommands::ExecuteCommandGetSkillRank");
    CNAME(0x082286D4, 0x08228B0F, "CNWVirtualMachineCommands::ExecuteCommandGetIsSkillSuccessful");
    CNAME(0x082143EC, 0x082146AC, "CNWVirtualMachineCommands::ExecuteCommandGetIsDoorActionPossible");
    CNAME(0x08053054, 0x0805376B, "CNWSPlayerCharSheetGUI::ComputeCharacterSheetUpdateRequired");
    CNAME(0x080580BC, 0x0805D90F, "CNWSPlayer::ValidateCharacter");
    CNAME(0x081D1CD4, 0x081D1FEB, "CNWSObject::BroadcastCounterSpellData");
    CNAME(0x081C53D0, 0x081C6397, "CNWSObject::AIActionUnlockObject");
    CNAME(0x081C6398, 0x081C7273, "CNWSObject::AIActionLockObject");
    CNAME(0x0806DB28, 0x0806FBEB, "CNWSMessage::WriteGameObjUpdate_PlayerUpdate");
    CNAME(0x08074DB8, 0x0807595F, "CNWSMessage::WriteGameObjUpdate_CharacterSheet");
    CNAME(0x080747D0, 0x08074C4F, "CNWSMessage::SendServerToPlayerExamineGui_TrapData");
    CNAME(0x081AE74C, 0x081AF1A3, "CNWSItemPropertyHandler::ApplyHolyAvenger");
    CNAME(0x0816C7E4, 0x0816DC57, "CNWSEffectListHandler::OnApplyDamage");
    CNAME(0x081579CC, 0x08158350, "CNWSCreatureStats::ValidateLevelUp");
    CNAME(0x0814A47C, 0x0814A8EF, "CNWSCreatureStats::UpdateLastStatsObject");
    CNAME(0x08150964, 0x08151713, "CNWSCreatureStats::SaveClassInfo");
    CNAME(0x0814C578, 0x0814F8DB, "CNWSCreatureStats::ReadStatsFromGff");
    CNAME(0x0815A0C4, 0x0815BC7F, "CNWSCreatureStats::LevelUpAutomatic");
    CNAME(0x081590F0, 0x0815986E, "CNWSCreatureStats::LevelUp");
    CNAME(0x08159870, 0x0815A0C3, "CNWSCreatureStats::LevelDown");
    CNAME(0x081656D8, 0x08165719, "CNWSCreatureStats::GetTotalACSkillMod");
    CNAME(0x08158748, 0x0815904D, "CNWSCreatureStats::GetMeetsPrestigeClassRequirements");
    CNAME(0x0815FB84, 0x0815FD22, "CNWSCreatureStats::GetCreatureTalentRandomSkill");
    CNAME(0x08155888, 0x08155B15, "CNWSCreatureStats::GetCreatureHasTalent");
    CNAME(0x08161918, 0x08161AB3, "CNWSCreatureStats::GetCanUseSkillAfterLevelUp");
    CNAME(0x08152E1C, 0x08152F5A, "CNWSCreatureStats::GetCanUseSkill");
    CNAME(0x0814088C, 0x08141CB9, "CNWSCreatureStats::GetArmorClassVersus");
    CNAME(0x08161160, 0x08161914, "CNWSCreatureStats::FeatRequirementsMetAfterLevelUp");
    CNAME(0x0815BF44, 0x0815C533, "CNWSCreatureStats::FeatRequirementsMet");
    CNAME(0x08131680, 0x0813179F, "CNWSCreature::UseLoreOnItem");
    CNAME(0x0811A2F4, 0x0811AF4A, "CNWSCreature::UpdateTrapCheck");
    CNAME(0x080F0A90, 0x080F182F, "CNWSCreature::SavingThrowRoll");
    CNAME(0x080E71D4, 0x080E762F, "CNWSCreature::ResolveRangedSpecialAttack");
    CNAME(0x080E9A98, 0x080EAA17, "CNWSCreature::ResolveMeleeSpecialAttack");
    CNAME(0x080EB190, 0x080EB71F, "CNWSCreature::ResolveAttackRoll");
    CNAME(0x081265C4, 0x08126B56, "CNWSCreature::DoSpotDetection");
    CNAME(0x08125DDC, 0x081265C2, "CNWSCreature::DoListenDetection");
    CNAME(0x0811172C, 0x0811192F, "CNWSCreature::CheckUseMagicDeviceSkill");
    CNAME(0x08127CEC, 0x08128863, "CNWSCreature::BroadcastSpellCast");
    CNAME(0x08128EA8, 0x081298AF, "CNWSCreature::BroadcastAttackOfOpportunity");
    CNAME(0x08109BEC, 0x0810A23B, "CNWSCreature::AIActionTaunt");
    CNAME(0x080F8174, 0x080F93D7, "CNWSCreature::AIActionSetTrap");
    CNAME(0x080F63A8, 0x080F706F, "CNWSCreature::AIActionRecoverTrap");
    CNAME(0x0810A3F4, 0x0810B3F7, "CNWSCreature::AIActionPickPocket");
    CNAME(0x0810B3F8, 0x0810BDDB, "CNWSCreature::AIActionHeal");
    CNAME(0x080F5D3C, 0x080F63A7, "CNWSCreature::AIActionFlagTrap");
    CNAME(0x080F7B78, 0x080F8173, "CNWSCreature::AIActionExamineTrap");
    CNAME(0x080F7070, 0x080F7B77, "CNWSCreature::AIActionDisarmTrap");
    CNAME(0x081E16B8, 0x081E1957, "CNWSCreature::AIActionCastSpell");
    CNAME(0x08108AFC, 0x08109113, "CNWSCreature::AIActionAnimalEmpathy");

    return "MISSING FUNCTION";
#undef CNAME
}

int8_t Hook_GetSkillRank(CNWSCreatureStats *stats, uint8_t skill, CNWSObject *vs, int32_t base){
    asm ("movl 0x4(%ebp), %eax");
    asm ("movl  %eax, pESP");
    
    unsigned long addr = (unsigned long)pESP;
    combat.Log(5, "Hook_GetSkillRank caller = %08lX (%s)\n", addr, GetFunctionCallerName(addr));
    int8_t result = 0;
    if ( CheckAddress(addr) ) {
        CNWSCreature *cre = stats->cs_original;
        auto c = combat.get_creature(cre->obj.obj_id);
        if ( !c ) { return 0; }
        result = c->getSkillRank(skill, vs, !!base);
    }
    else {
        result = CNWSCreatureStats__GetSkillRank_orig(stats, skill, vs, base);
    }

    combat.Log(3, "Hook_GetSkillRank: Creature: %x, versus: %x, skill: %d, base: %d, result: %d\n",
               stats->cs_original->obj.obj_id,
               vs ? vs->obj_id : OBJECT_INVALID,
               skill, base, result);
    
    return result;
}
