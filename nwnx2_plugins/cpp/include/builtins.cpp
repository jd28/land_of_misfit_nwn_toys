#include "nx_devel.h"
#include "nx_safe.h"
#include "nx_log.h"
#include "nx_hook.h"
#include "nx_signature.h"

#include "ScriptLib.h"
#include "types.h"
#include "consts.h"

int Random(int nMaxInteger)
{
    nwn_StackPushInteger(nMaxInteger);
    nwn_ExecuteCommand(0, 1);
    return nwn_StackPopInteger();
}

void PrintString(const string& sString)
{
    nwn_StackPushString(sString.c_str());
    nwn_ExecuteCommand(1, 1);
}

void PrintFloat(float fFloat, int nWidth, int nDecimals)
{
    nwn_StackPushInteger(nDecimals);
    nwn_StackPushInteger(nWidth);
    nwn_StackPushFloat(fFloat);
    nwn_ExecuteCommand(2, 3);
}

string FloatToString(float fFloat, int nWidth, int nDecimals)
{
    nwn_StackPushInteger(nDecimals);
    nwn_StackPushInteger(nWidth);
    nwn_StackPushFloat(fFloat);
    nwn_ExecuteCommand(3, 3);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

void PrintInteger(int nInteger)
{
    nwn_StackPushInteger(nInteger);
    nwn_ExecuteCommand(4, 1);
}

void PrintObject(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(5, 1);
}

void ExecuteScript(const string& sScript, object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_StackPushString(sScript.c_str());
    nwn_ExecuteCommand(8, 2);
}

void ClearAllActions(int nClearCombatState)
{
    nwn_StackPushInteger(nClearCombatState);
    nwn_ExecuteCommand(9, 1);
}

void SetFacing(float fDirection)
{
    nwn_StackPushFloat(fDirection);
    nwn_ExecuteCommand(10, 1);
}

void SetCalendar(int nYear, int nMonth, int nDay)
{
    nwn_StackPushInteger(nDay);
    nwn_StackPushInteger(nMonth);
    nwn_StackPushInteger(nYear);
    nwn_ExecuteCommand(11, 3);
}

void SetTime(int nHour, int nMinute, int nSecond, int nMillisecond)
{
    nwn_StackPushInteger(nMillisecond);
    nwn_StackPushInteger(nSecond);
    nwn_StackPushInteger(nMinute);
    nwn_StackPushInteger(nHour);
    nwn_ExecuteCommand(12, 4);
}

int GetCalendarYear()
{
    nwn_ExecuteCommand(13, 0);
    return nwn_StackPopInteger();
}

int GetCalendarMonth()
{
    nwn_ExecuteCommand(14, 0);
    return nwn_StackPopInteger();
}

int GetCalendarDay()
{
    nwn_ExecuteCommand(15, 0);
    return nwn_StackPopInteger();
}

int GetTimeHour()
{
    nwn_ExecuteCommand(16, 0);
    return nwn_StackPopInteger();
}

int GetTimeMinute()
{
    nwn_ExecuteCommand(17, 0);
    return nwn_StackPopInteger();
}

int GetTimeSecond()
{
    nwn_ExecuteCommand(18, 0);
    return nwn_StackPopInteger();
}

int GetTimeMillisecond()
{
    nwn_ExecuteCommand(19, 0);
    return nwn_StackPopInteger();
}

void ActionRandomWalk()
{
    nwn_ExecuteCommand(20, 0);
}

void ActionMoveToLocation(const location& lDestination, int bRun)
{
    nwn_StackPushInteger(bRun);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lDestination);
    nwn_ExecuteCommand(21, 2);
}

void ActionMoveToObject(object oMoveTo, int bRun, float fRange)
{
    nwn_StackPushFloat(fRange);
    nwn_StackPushInteger(bRun);
    nwn_StackPushObject(oMoveTo);
    nwn_ExecuteCommand(22, 3);
}

void ActionMoveAwayFromObject(object oFleeFrom, int bRun, float fMoveAwayRange)
{
    nwn_StackPushFloat(fMoveAwayRange);
    nwn_StackPushInteger(bRun);
    nwn_StackPushObject(oFleeFrom);
    nwn_ExecuteCommand(23, 3);
}

object GetArea(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(24, 1);
    return nwn_StackPopObject();
}

object GetEnteringObject()
{
    nwn_ExecuteCommand(25, 0);
    return nwn_StackPopObject();
}

object GetExitingObject()
{
    nwn_ExecuteCommand(26, 0);
    return nwn_StackPopObject();
}

vec3 GetPosition(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(27, 1);
    Vector *v = nwn_StackPopVector();
    vec3 res { v->X, v->Y, v->Z };
    free(v);
    return res;
}

float GetFacing(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(28, 1);
    return nwn_StackPopFloat();
}

object GetItemPossessor(object oItem)
{
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(29, 1);
    return nwn_StackPopObject();
}

object GetItemPossessedBy(object oCreature, const string& sItemTag)
{
    nwn_StackPushString(sItemTag.c_str());
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(30, 2);
    return nwn_StackPopObject();
}

object CreateItemOnObject(const string& sItemTemplate, object oTarget,
                          int nStackSize, const string& sNewTag)
{
    nwn_StackPushString(sNewTag.c_str());
    nwn_StackPushInteger(nStackSize);
    nwn_StackPushObject(oTarget);
    nwn_StackPushString(sItemTemplate.c_str());
    nwn_ExecuteCommand(31, 4);
    return nwn_StackPopObject();
}

void ActionEquipItem(object oItem, int nInventorySlot)
{
    nwn_StackPushInteger(nInventorySlot);
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(32, 2);
}

void ActionUnequipItem(object oItem)
{
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(33, 1);
}

void ActionPickUpItem(object oItem)
{
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(34, 1);
}

void ActionPutDownItem(object oItem)
{
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(35, 1);
}

object GetLastAttacker(object oAttackee)
{
    nwn_StackPushObject(oAttackee);
    nwn_ExecuteCommand(36, 1);
    return nwn_StackPopObject();
}

void ActionAttack(object oAttackee, int bPassive)
{
    nwn_StackPushInteger(bPassive);
    nwn_StackPushObject(oAttackee);
    nwn_ExecuteCommand(37, 2);
}

object GetNearestCreature(int nFirstCriteriaType, int nFirstCriteriaValue, object oTarget,
                          int nNth, int nSecondCriteriaType, int nSecondCriteriaValue,
                          int nThirdCriteriaType, int nThirdCriteriaValue)
{
    nwn_StackPushInteger(nThirdCriteriaValue);
    nwn_StackPushInteger(nThirdCriteriaType);
    nwn_StackPushInteger(nSecondCriteriaValue);
    nwn_StackPushInteger(nSecondCriteriaType);
    nwn_StackPushInteger(nNth);
    nwn_StackPushObject(oTarget);
    nwn_StackPushInteger(nFirstCriteriaValue);
    nwn_StackPushInteger(nFirstCriteriaType);
    nwn_ExecuteCommand(38, 8);
    return nwn_StackPopObject();
}

void ActionSpeakString(const string& sStringToSpeak, int nTalkVolume)
{
    nwn_StackPushInteger(nTalkVolume);
    nwn_StackPushString(sStringToSpeak.c_str());
    nwn_ExecuteCommand(39, 2);
}

void ActionPlayAnimation(int nAnimation, float fSpeed, float fDurationSeconds)
{
    nwn_StackPushFloat(fDurationSeconds);
    nwn_StackPushFloat(fSpeed);
    nwn_StackPushInteger(nAnimation);
    nwn_ExecuteCommand(40, 3);
}

float GetDistanceToObject(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(41, 1);
    return nwn_StackPopFloat();
}

int GetIsObjectValid(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(42, 1);
    return nwn_StackPopInteger();
}

void ActionOpenDoor(object oDoor)
{
    nwn_StackPushObject(oDoor);
    nwn_ExecuteCommand(43, 1);
}

void ActionCloseDoor(object oDoor)
{
    nwn_StackPushObject(oDoor);
    nwn_ExecuteCommand(44, 1);
}

void SetCameraFacing(float fDirection, float fDistance, float fPitch, int nTransitionType)
{
    nwn_StackPushInteger(nTransitionType);
    nwn_StackPushFloat(fPitch);
    nwn_StackPushFloat(fDistance);
    nwn_StackPushFloat(fDirection);
    nwn_ExecuteCommand(45, 4);
}

void PlaySound(const string& sSoundName)
{
    nwn_StackPushString(sSoundName.c_str());
    nwn_ExecuteCommand(46, 1);
}

object GetSpellTargetObject()
{
    nwn_ExecuteCommand(47, 0);
    return nwn_StackPopObject();
}

void ActionCastSpellAtObject(int nSpell, object oTarget, int nMetaMagic, int bCheat,
                             int nDomainLevel,
                             int nProjectilePathType, int bInstantSpell)
{
    nwn_StackPushInteger(bInstantSpell);
    nwn_StackPushInteger(nProjectilePathType);
    nwn_StackPushInteger(nDomainLevel);
    nwn_StackPushInteger(bCheat);
    nwn_StackPushInteger(nMetaMagic);
    nwn_StackPushObject(oTarget);
    nwn_StackPushInteger(nSpell);
    nwn_ExecuteCommand(48, 7);
}

int GetCurrentHitPoints(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(49, 1);
    return nwn_StackPopInteger();
}

int GetMaxHitPoints(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(50, 1);
    return nwn_StackPopInteger();
}

int GetLocalInt(object oObject, const string& sVarName)
{
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(51, 2);
    return nwn_StackPopInteger();
}

float GetLocalFloat(object oObject, const string& sVarName)
{
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(52, 2);
    return nwn_StackPopFloat();
}

string GetLocalString(object oObject, const string& sVarName)
{
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(53, 2);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

object GetLocalObject(object oObject, const string& sVarName)
{
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(54, 2);
    return nwn_StackPopObject();
}

void SetLocalInt(object oObject, const string& sVarName, int nValue)
{
    nwn_StackPushInteger(nValue);
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(55, 3);
}

void SetLocalFloat(object oObject, const string& sVarName, float fValue)
{
    nwn_StackPushFloat(fValue);
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(56, 3);
}

void SetLocalString(object oObject, const string& sVarName, const string& sValue)
{
    nwn_StackPushString(sValue.c_str());
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(57, 3);
}

void SetLocalObject(object oObject, const string& sVarName, object oValue)
{
    nwn_StackPushObject(oValue);
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(58, 3);
}

int GetStringLength(const string& sString)
{
    return sString.size();
}

string GetStringUpperCase(const string& sString)
{
    nwn_StackPushString(sString.c_str());
    nwn_ExecuteCommand(60, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

string GetStringLowerCase(const string& sString)
{
    nwn_StackPushString(sString.c_str());
    nwn_ExecuteCommand(61, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

string GetStringRight(const string& sString, int nCount)
{
    nwn_StackPushInteger(nCount);
    nwn_StackPushString(sString.c_str());
    nwn_ExecuteCommand(62, 2);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

string GetStringLeft(const string& sString, int nCount)
{
    nwn_StackPushInteger(nCount);
    nwn_StackPushString(sString.c_str());
    nwn_ExecuteCommand(63, 2);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

string InsertString(const string& sDestination, const string& sString, int nPosition)
{
    nwn_StackPushInteger(nPosition);
    nwn_StackPushString(sString.c_str());
    nwn_StackPushString(sDestination.c_str());
    nwn_ExecuteCommand(64, 3);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

string GetSubString(const string& sString, int nStart, int nCount)
{
    nwn_StackPushInteger(nCount);
    nwn_StackPushInteger(nStart);
    nwn_StackPushString(sString.c_str());
    nwn_ExecuteCommand(65, 3);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

int FindSubString(const string& sString, const string& sSubString, int nStart)
{
    nwn_StackPushInteger(nStart);
    nwn_StackPushString(sSubString.c_str());
    nwn_StackPushString(sString.c_str());
    nwn_ExecuteCommand(66, 3);
    return nwn_StackPopInteger();
}

effect EffectHeal(int nDamageToHeal)
{
    nwn_StackPushInteger(nDamageToHeal);
    nwn_ExecuteCommand(78, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectDamage(int nDamageAmount, int nDamageType, int nDamagePower)
{
    nwn_StackPushInteger(nDamagePower);
    nwn_StackPushInteger(nDamageType);
    nwn_StackPushInteger(nDamageAmount);
    nwn_ExecuteCommand(79, 3);
    return effect(nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT));
}

effect EffectAbilityIncrease(int nAbilityToIncrease, int nModifyBy)
{
    nwn_StackPushInteger(nModifyBy);
    nwn_StackPushInteger(nAbilityToIncrease);
    nwn_ExecuteCommand(80, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectDamageResistance(int nDamageType, int nAmount, int nLimit)
{
    nwn_StackPushInteger(nLimit);
    nwn_StackPushInteger(nAmount);
    nwn_StackPushInteger(nDamageType);
    nwn_ExecuteCommand(81, 3);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectResurrection()
{
    nwn_ExecuteCommand(82, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectSummonCreature(const string& sCreatureResref, int nVisualEffectId, float fDelaySeconds,
                            int nUseAppearAnimation)
{
    nwn_StackPushInteger(nUseAppearAnimation);
    nwn_StackPushFloat(fDelaySeconds);
    nwn_StackPushInteger(nVisualEffectId);
    nwn_StackPushString(sCreatureResref.c_str());
    nwn_ExecuteCommand(83, 4);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

int GetCasterLevel(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(84, 1);
    return nwn_StackPopInteger();
}

effect GetFirstEffect(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(85, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect GetNextEffect(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(86, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

void RemoveEffect(object oCreature, const effect& eEffect)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, eEffect.structure);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(87, 2);
}

int GetIsEffectValid(const effect& eEffect)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, eEffect.structure);
    nwn_ExecuteCommand(88, 1);
    return nwn_StackPopInteger();
}

int GetEffectDurationType(const effect& eEffect)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, eEffect.structure);
    nwn_ExecuteCommand(89, 1);
    return nwn_StackPopInteger();
}

int GetEffectSubType(const effect& eEffect)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, eEffect.structure);
    nwn_ExecuteCommand(90, 1);
    return nwn_StackPopInteger();
}

object GetEffectCreator(const effect& eEffect)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, eEffect.structure);
    nwn_ExecuteCommand(91, 1);
    return nwn_StackPopObject();
}

string IntToString(int nInteger)
{
    nwn_StackPushInteger(nInteger);
    nwn_ExecuteCommand(92, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

object GetFirstObjectInArea(object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(93, 1);
    return nwn_StackPopObject();
}

object GetNextObjectInArea(object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(94, 1);
    return nwn_StackPopObject();
}

int d2(int nNumDice)
{
    nwn_StackPushInteger(nNumDice);
    nwn_ExecuteCommand(95, 1);
    return nwn_StackPopInteger();
}

int d3(int nNumDice)
{
    nwn_StackPushInteger(nNumDice);
    nwn_ExecuteCommand(96, 1);
    return nwn_StackPopInteger();
}

int d4(int nNumDice)
{
    nwn_StackPushInteger(nNumDice);
    nwn_ExecuteCommand(97, 1);
    return nwn_StackPopInteger();
}

int d6(int nNumDice)
{
    nwn_StackPushInteger(nNumDice);
    nwn_ExecuteCommand(98, 1);
    return nwn_StackPopInteger();
}

int d8(int nNumDice)
{
    nwn_StackPushInteger(nNumDice);
    nwn_ExecuteCommand(99, 1);
    return nwn_StackPopInteger();
}

int d10(int nNumDice)
{
    nwn_StackPushInteger(nNumDice);
    nwn_ExecuteCommand(100, 1);
    return nwn_StackPopInteger();
}

int d12(int nNumDice)
{
    nwn_StackPushInteger(nNumDice);
    nwn_ExecuteCommand(101, 1);
    return nwn_StackPopInteger();
}

int d20(int nNumDice)
{
    nwn_StackPushInteger(nNumDice);
    nwn_ExecuteCommand(102, 1);
    return nwn_StackPopInteger();
}

int d100(int nNumDice)
{
    nwn_StackPushInteger(nNumDice);
    nwn_ExecuteCommand(103, 1);
    return nwn_StackPopInteger();
}

float vec3Magnitude(vec3 vvec3)
{
    nwn_StackPushVector((Vector*)&vvec3);
    nwn_ExecuteCommand(104, 1);
    return nwn_StackPopFloat();
}

int GetMetaMagicFeat()
{
    nwn_ExecuteCommand(105, 0);
    return nwn_StackPopInteger();
}

int GetObjectType(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(106, 1);
    return nwn_StackPopInteger();
}

int GetRacialType(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(107, 1);
    return nwn_StackPopInteger();
}

int FortitudeSave(object oCreature, int nDC, int nSaveType, object oSaveVersus)
{
    nwn_StackPushObject(oSaveVersus);
    nwn_StackPushInteger(nSaveType);
    nwn_StackPushInteger(nDC);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(108, 4);
    return nwn_StackPopInteger();
}

int ReflexSave(object oCreature, int nDC, int nSaveType, object oSaveVersus)
{
    nwn_StackPushObject(oSaveVersus);
    nwn_StackPushInteger(nSaveType);
    nwn_StackPushInteger(nDC);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(109, 4);
    return nwn_StackPopInteger();
}

int WillSave(object oCreature, int nDC, int nSaveType, object oSaveVersus)
{
    nwn_StackPushObject(oSaveVersus);
    nwn_StackPushInteger(nSaveType);
    nwn_StackPushInteger(nDC);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(110, 4);
    return nwn_StackPopInteger();
}

int GetSpellSaveDC()
{
    nwn_ExecuteCommand(111, 0);
    return nwn_StackPopInteger();
}

effect MagicalEffect(const effect& eEffect)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, eEffect.structure);
    nwn_ExecuteCommand(112, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect SupernaturalEffect(const effect& eEffect)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, eEffect.structure);
    nwn_ExecuteCommand(113, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect ExtraordinaryEffect(const effect& eEffect)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, eEffect.structure);
    nwn_ExecuteCommand(114, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectACIncrease(int nValue, int nModifyType, int nDamageType)
{
    nwn_StackPushInteger(nDamageType);
    nwn_StackPushInteger(nModifyType);
    nwn_StackPushInteger(nValue);
    nwn_ExecuteCommand(115, 3);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

int GetAC(object oObject, int nForFutureUse)
{
    nwn_StackPushInteger(nForFutureUse);
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(116, 2);
    return nwn_StackPopInteger();
}

effect EffectSavingThrowIncrease(int nSave, int nValue, int nSaveType)
{
    nwn_StackPushInteger(nSaveType);
    nwn_StackPushInteger(nValue);
    nwn_StackPushInteger(nSave);
    nwn_ExecuteCommand(117, 3);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectAttackIncrease(int nBonus, int nModifierType)
{
    nwn_StackPushInteger(nModifierType);
    nwn_StackPushInteger(nBonus);
    nwn_ExecuteCommand(118, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectDamageReduction(int nAmount, int nDamagePower, int nLimit)
{
    nwn_StackPushInteger(nLimit);
    nwn_StackPushInteger(nDamagePower);
    nwn_StackPushInteger(nAmount);
    nwn_ExecuteCommand(119, 3);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectDamageIncrease(int nBonus, int nDamageType)
{
    nwn_StackPushInteger(nDamageType);
    nwn_StackPushInteger(nBonus);
    nwn_ExecuteCommand(120, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

float RoundsToSeconds(int nRounds)
{
    return nRounds * 6.0f;
}

float HoursToSeconds(int nHours)
{
    nwn_StackPushInteger(nHours);
    nwn_ExecuteCommand(122, 1);
    return nwn_StackPopFloat();
}

float TurnsToSeconds(int nTurns)
{
    return 60.0f * nTurns;
}

int GetLawChaosValue(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(124, 1);
    return nwn_StackPopInteger();
}

int GetGoodEvilValue(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(125, 1);
    return nwn_StackPopInteger();
}

int GetAlignmentLawChaos(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(126, 1);
    return nwn_StackPopInteger();
}

int GetAlignmentGoodEvil(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(127, 1);
    return nwn_StackPopInteger();
}

object GetFirstObjectInShape(int nShape, float fSize, const location& lTarget, int bLineOfSight,
                             int nObjectFilter, vec3 vOrigin)
{
    return OBJECT_INVALID;
}

object GetNextObjectInShape(int nShape, float fSize, const location& lTarget, int bLineOfSight,
                            int nObjectFilter, vec3 vOrigin)
{
    return OBJECT_INVALID;
}

effect EffectEntangle()
{
    nwn_ExecuteCommand(130, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

void SignalEvent(object oObject, const event& eEvent)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_EVENT, eEvent.structure);
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(131, 2);
}

event EventUserDefined(int nUserDefinedEventNumber)
{
    nwn_StackPushInteger(nUserDefinedEventNumber);
    nwn_ExecuteCommand(132, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EVENT);
}

effect EffectDeath(int nSpectacularDeath, int nDisplayFeedback)
{
    nwn_StackPushInteger(nDisplayFeedback);
    nwn_StackPushInteger(nSpectacularDeath);
    nwn_ExecuteCommand(133, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectKnockdown()
{
    nwn_ExecuteCommand(134, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

void ActionGiveItem(object oItem, object oGiveTo)
{
    nwn_StackPushObject(oGiveTo);
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(135, 2);
}

void ActionTakeItem(object oItem, object oTakeFrom)
{
    nwn_StackPushObject(oTakeFrom);
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(136, 2);
}

vec3 vec3Normalize(vec3 vvec3)
{
    nwn_StackPushVector((Vector*)&vvec3);
    nwn_ExecuteCommand(137, 1);
    Vector *v = nwn_StackPopVector();
    vec3 res = { v->X, v->Y, v->Z };
    free(v);
    return res;
}

effect EffectCurse(int nStrMod, int nDexMod, int nConMod, int nIntMod, int nWisMod, int nChaMod)
{
    nwn_StackPushInteger(nChaMod);
    nwn_StackPushInteger(nWisMod);
    nwn_StackPushInteger(nIntMod);
    nwn_StackPushInteger(nConMod);
    nwn_StackPushInteger(nDexMod);
    nwn_StackPushInteger(nStrMod);
    nwn_ExecuteCommand(138, 6);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

int GetAbilityScore(object oCreature, int nAbilityType, int nBaseAbilityScore)
{
    nwn_StackPushInteger(nBaseAbilityScore);
    nwn_StackPushInteger(nAbilityType);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(139, 3);
    return nwn_StackPopInteger();
}

int GetIsDead(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(140, 1);
    return nwn_StackPopInteger();
}

void PrintVector(vec3 vvec3, int bPrepend)
{
    nwn_StackPushInteger(bPrepend);
    nwn_StackPushVector((Vector*)&vvec3);
    nwn_ExecuteCommand(141, 2);
}

void SetFacingPoint(vec3 vTarget)
{
    nwn_StackPushVector((Vector*)&vTarget);
    nwn_ExecuteCommand(143, 1);
}
/*
float vec3ToAngle(vec3 vvec3) {
  nwn_StackPushFloat(fAngle);
  nwn_ExecuteCommand(144, 1);
  return nwn_StackPopVector();
}
*/
float VectorToAngle(vec3 v)
{
    nwn_StackPushVector((Vector*)&v);
    nwn_ExecuteCommand(145, 1);
    return nwn_StackPopFloat();
}

int TouchAttackMelee(object oTarget, int bDisplayFeedback)
{
    nwn_StackPushInteger(bDisplayFeedback);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(146, 2);
    return nwn_StackPopInteger();
}

int TouchAttackRanged(object oTarget, int bDisplayFeedback)
{
    nwn_StackPushInteger(bDisplayFeedback);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(147, 2);
    return nwn_StackPopInteger();
}

effect EffectParalyze()
{
    nwn_ExecuteCommand(148, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectSpellImmunity(int nImmunityToSpell)
{
    nwn_StackPushInteger(nImmunityToSpell);
    nwn_ExecuteCommand(149, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectDeaf()
{
    nwn_ExecuteCommand(150, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

float GetDistanceBetween(object oObjectA, object oObjectB)
{
    nwn_StackPushObject(oObjectB);
    nwn_StackPushObject(oObjectA);
    nwn_ExecuteCommand(151, 2);
    return nwn_StackPopFloat();
}

void SetLocalLocation(object oObject, const string& sVarName, const location& lValue)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lValue);
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(152, 3);
}

location GetLocalLocation(object oObject, const string& sVarName)
{
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(153, 2);

    CScriptLocation* l = (CScriptLocation*)nwn_StackPopEngineStructure(ENGINE_STRUCTURE_LOCATION);
    location loc { { l->X, l->Y, l->Z },
        { l->OrientationX, l->OrientationY, l->OrientationZ },
        l->AreaID
    };
    free(l);
    return loc;
}

effect EffectSleep()
{
    nwn_ExecuteCommand(154, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

object GetItemInSlot(int nInventorySlot, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nInventorySlot);
    nwn_ExecuteCommand(155, 2);
    return nwn_StackPopObject();
}

effect EffectCharmed()
{
    nwn_ExecuteCommand(156, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectConfused()
{
    nwn_ExecuteCommand(157, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectFrightened()
{
    nwn_ExecuteCommand(158, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectDominated()
{
    nwn_ExecuteCommand(159, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectDazed()
{
    nwn_ExecuteCommand(160, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectStunned()
{
    nwn_ExecuteCommand(161, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

void SetCommandable(int bCommandable, object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_StackPushInteger(bCommandable);
    nwn_ExecuteCommand(162, 2);
}

int GetCommandable(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(163, 1);
    return nwn_StackPopInteger();
}

effect EffectRegenerate(int nAmount, float fIntervalSeconds)
{
    nwn_StackPushFloat(fIntervalSeconds);
    nwn_StackPushInteger(nAmount);
    nwn_ExecuteCommand(164, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectMovementSpeedIncrease(int nPercentChange)
{
    nwn_StackPushInteger(nPercentChange);
    nwn_ExecuteCommand(165, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

int GetHitDice(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(166, 1);
    return nwn_StackPopInteger();
}

void ActionForceFollowObject(object oFollow, float fFollowDistance)
{
    nwn_StackPushFloat(fFollowDistance);
    nwn_StackPushObject(oFollow);
    nwn_ExecuteCommand(167, 2);
}

string GetTag(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(168, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

int ResistSpell(object oCaster, object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_StackPushObject(oCaster);
    nwn_ExecuteCommand(169, 2);
    return nwn_StackPopInteger();
}

int GetEffectType(const effect& eEffect)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, eEffect.structure);
    nwn_ExecuteCommand(170, 1);
    return nwn_StackPopInteger();
}

effect EffectAreaOfEffect(int nAreaEffectId, const string& sOnEnterScript,
                          const string& sHeartbeatScript,
                          const string& sOnExitScript)
{
    nwn_StackPushString(sOnExitScript.c_str());
    nwn_StackPushString(sHeartbeatScript.c_str());
    nwn_StackPushString(sOnEnterScript.c_str());
    nwn_StackPushInteger(nAreaEffectId);
    nwn_ExecuteCommand(171, 4);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

int GetFactionEqual(object oFirstObject, object oSecondObject)
{
    nwn_StackPushObject(oSecondObject);
    nwn_StackPushObject(oFirstObject);
    nwn_ExecuteCommand(172, 2);
    return nwn_StackPopInteger();
}

void ChangeFaction(object oObjectToChangeFaction, object oMemberOfFactionToJoin)
{
    nwn_StackPushObject(oMemberOfFactionToJoin);
    nwn_StackPushObject(oObjectToChangeFaction);
    nwn_ExecuteCommand(173, 2);
}

int GetIsListening(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(174, 1);
    return nwn_StackPopInteger();
}

void SetListening(object oObject, int bValue)
{
    nwn_StackPushInteger(bValue);
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(175, 2);
}

void SetListenPattern(object oObject, const string& sPattern, int nNumber)
{
    nwn_StackPushInteger(nNumber);
    nwn_StackPushString(sPattern.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(176, 3);
}

int TestStringAgainstPattern(const string& sPattern, const string& sStringToTest)
{
    nwn_StackPushString(sStringToTest.c_str());
    nwn_StackPushString(sPattern.c_str());
    nwn_ExecuteCommand(177, 2);
    return nwn_StackPopInteger();
}

string GetMatchedSubstring(int nString)
{
    nwn_StackPushInteger(nString);
    nwn_ExecuteCommand(178, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

int GetMatchedSubstringsCount()
{
    nwn_ExecuteCommand(179, 0);
    return nwn_StackPopInteger();
}

effect EffectVisualEffect(int nVisualEffectId, int nMissEffect)
{
    nwn_StackPushInteger(nMissEffect);
    nwn_StackPushInteger(nVisualEffectId);
    nwn_ExecuteCommand(180, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

object GetFactionWeakestMember(object oFactionMember, int bMustBeVisible)
{
    nwn_StackPushInteger(bMustBeVisible);
    nwn_StackPushObject(oFactionMember);
    nwn_ExecuteCommand(181, 2);
    return nwn_StackPopObject();
}

object GetFactionStrongestMember(object oFactionMember, int bMustBeVisible)
{
    nwn_StackPushInteger(bMustBeVisible);
    nwn_StackPushObject(oFactionMember);
    nwn_ExecuteCommand(182, 2);
    return nwn_StackPopObject();
}

object GetFactionMostDamagedMember(object oFactionMember, int bMustBeVisible)
{
    nwn_StackPushInteger(bMustBeVisible);
    nwn_StackPushObject(oFactionMember);
    nwn_ExecuteCommand(183, 2);
    return nwn_StackPopObject();
}

object GetFactionLeastDamagedMember(object oFactionMember, int bMustBeVisible)
{
    nwn_StackPushInteger(bMustBeVisible);
    nwn_StackPushObject(oFactionMember);
    nwn_ExecuteCommand(184, 2);
    return nwn_StackPopObject();
}

int GetFactionGold(object oFactionMember)
{
    nwn_StackPushObject(oFactionMember);
    nwn_ExecuteCommand(185, 1);
    return nwn_StackPopInteger();
}

int GetFactionAverageReputation(object oSourceFactionMember, object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_StackPushObject(oSourceFactionMember);
    nwn_ExecuteCommand(186, 2);
    return nwn_StackPopInteger();
}

int GetFactionAverageGoodEvilAlignment(object oFactionMember)
{
    nwn_StackPushObject(oFactionMember);
    nwn_ExecuteCommand(187, 1);
    return nwn_StackPopInteger();
}

int GetFactionAverageLawChaosAlignment(object oFactionMember)
{
    nwn_StackPushObject(oFactionMember);
    nwn_ExecuteCommand(188, 1);
    return nwn_StackPopInteger();
}

int GetFactionAverageLevel(object oFactionMember)
{
    nwn_StackPushObject(oFactionMember);
    nwn_ExecuteCommand(189, 1);
    return nwn_StackPopInteger();
}

int GetFactionAverageXP(object oFactionMember)
{
    nwn_StackPushObject(oFactionMember);
    nwn_ExecuteCommand(190, 1);
    return nwn_StackPopInteger();
}

int GetFactionMostFrequentClass(object oFactionMember)
{
    nwn_StackPushObject(oFactionMember);
    nwn_ExecuteCommand(191, 1);
    return nwn_StackPopInteger();
}

object GetFactionWorstAC(object oFactionMember, int bMustBeVisible)
{
    nwn_StackPushInteger(bMustBeVisible);
    nwn_StackPushObject(oFactionMember);
    nwn_ExecuteCommand(192, 2);
    return nwn_StackPopObject();
}

object GetFactionBestAC(object oFactionMember, int bMustBeVisible)
{
    nwn_StackPushInteger(bMustBeVisible);
    nwn_StackPushObject(oFactionMember);
    nwn_ExecuteCommand(193, 2);
    return nwn_StackPopObject();
}

void ActionSit(object oChair)
{
    nwn_StackPushObject(oChair);
    nwn_ExecuteCommand(194, 1);
}

int GetListenPatternNumber()
{
    nwn_ExecuteCommand(195, 0);
    return nwn_StackPopInteger();
}

void ActionJumpToObject(object oToJumpTo, int bWalkStraightLineToPoint)
{
    nwn_StackPushInteger(bWalkStraightLineToPoint);
    nwn_StackPushObject(oToJumpTo);
    nwn_ExecuteCommand(196, 2);
}

object GetWaypointByTag(const string& sWaypointTag)
{
    nwn_StackPushString(sWaypointTag.c_str());
    nwn_ExecuteCommand(197, 1);
    return nwn_StackPopObject();
}

object GetTransitionTarget(object oTransition)
{
    nwn_StackPushObject(oTransition);
    nwn_ExecuteCommand(198, 1);
    return nwn_StackPopObject();
}

effect EffectLinkEffects(const effect& eChildEffect, const effect& eParentEffect)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, eParentEffect.structure);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, eChildEffect.structure);
    nwn_ExecuteCommand(199, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

object GetObjectByTag(const string& sTag, int nNth)
{
    nwn_StackPushInteger(nNth);
    nwn_StackPushString(sTag.c_str());
    nwn_ExecuteCommand(200, 2);
    return nwn_StackPopObject();
}

void AdjustAlignment(object oSubject, int nAlignment, int nShift, int bAllPartyMembers)
{
    nwn_StackPushInteger(bAllPartyMembers);
    nwn_StackPushInteger(nShift);
    nwn_StackPushInteger(nAlignment);
    nwn_StackPushObject(oSubject);
    nwn_ExecuteCommand(201, 4);
}

void ActionWait(float fSeconds)
{
    nwn_StackPushFloat(fSeconds);
    nwn_ExecuteCommand(202, 1);
}

void SetAreaTransitionBMP(int nPredefinedAreaTransition, const string& sCustomAreaTransitionBMP)
{
    nwn_StackPushString(sCustomAreaTransitionBMP.c_str());
    nwn_StackPushInteger(nPredefinedAreaTransition);
    nwn_ExecuteCommand(203, 2);
}

void ActionStartConversation(object oObjectToConverseWith, const string& sDialogResRef,
                             int bPrivateConversation, int bPlayHello)
{
    nwn_StackPushInteger(bPlayHello);
    nwn_StackPushInteger(bPrivateConversation);
    nwn_StackPushString(sDialogResRef.c_str());
    nwn_StackPushObject(oObjectToConverseWith);
    nwn_ExecuteCommand(204, 4);
}

void ActionPauseConversation()
{
    nwn_ExecuteCommand(205, 0);
}

void ActionResumeConversation()
{
    nwn_ExecuteCommand(206, 0);
}

effect EffectBeam(int nBeamVisualEffect, object oEffector, int nBodyPart, int bMissEffect)
{
    nwn_StackPushInteger(bMissEffect);
    nwn_StackPushInteger(nBodyPart);
    nwn_StackPushObject(oEffector);
    nwn_StackPushInteger(nBeamVisualEffect);
    nwn_ExecuteCommand(207, 4);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

int GetReputation(object oSource, object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_StackPushObject(oSource);
    nwn_ExecuteCommand(208, 2);
    return nwn_StackPopInteger();
}

void AdjustReputation(object oTarget, object oSourceFactionMember, int nAdjustment)
{
    nwn_StackPushInteger(nAdjustment);
    nwn_StackPushObject(oSourceFactionMember);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(209, 3);
}

object GetSittingCreature(object oChair)
{
    nwn_StackPushObject(oChair);
    nwn_ExecuteCommand(210, 1);
    return nwn_StackPopObject();
}

object GetGoingToBeAttackedBy(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(211, 1);
    return nwn_StackPopObject();
}

effect EffectSpellResistanceIncrease(int nValue)
{
    nwn_StackPushInteger(nValue);
    nwn_ExecuteCommand(212, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

location GetLocation(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(213, 1);
    CScriptLocation* l = (CScriptLocation*)nwn_StackPopEngineStructure(ENGINE_STRUCTURE_LOCATION);
    location loc { { l->X, l->Y, l->Z },
        { l->OrientationX, l->OrientationY, l->OrientationZ },
        l->AreaID
    };
    free(l);
    return loc;

}

void ActionJumpToLocation(const location& lLocation)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lLocation);
    nwn_ExecuteCommand(214, 1);
}

location Location(object oArea, vec3 vPosition, float fOrientation)
{
    nwn_StackPushFloat(fOrientation);
    nwn_StackPushVector((Vector*)&vPosition);
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(215, 3);
    CScriptLocation* l = (CScriptLocation*)nwn_StackPopEngineStructure(ENGINE_STRUCTURE_LOCATION);
    location loc { { l->X, l->Y, l->Z },
        { l->OrientationX, l->OrientationY, l->OrientationZ },
        l->AreaID
    };
    free(l);
    return loc;

}

void ApplyEffectAtLocation(int nDurationType, const effect& eEffect, const location& lLocation,
                           float fDuration)
{
    nwn_StackPushFloat(fDuration);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lLocation);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, eEffect.structure);
    nwn_StackPushInteger(nDurationType);
    nwn_ExecuteCommand(216, 4);
}

int GetIsPC(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(217, 1);
    return nwn_StackPopInteger();
}

float FeetToMeters(float fFeet)
{
    nwn_StackPushFloat(fFeet);
    nwn_ExecuteCommand(218, 1);
    return nwn_StackPopFloat();
}

float YardsToMeters(float fYards)
{
    nwn_StackPushFloat(fYards);
    nwn_ExecuteCommand(219, 1);
    return nwn_StackPopFloat();
}

void ApplyEffectToObject(int nDurationType, const effect& eEffect, object oTarget, float fDuration)
{
    nwn_StackPushFloat(fDuration);
    nwn_StackPushObject(oTarget);
    nx_log(NX_LOG_INFO, 0, "Applying effect: %p to %x", eEffect.structure, oTarget);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, eEffect.structure);
    nwn_StackPushInteger(nDurationType);
    nwn_ExecuteCommand(220, 4);
}

void SpeakString(const string& sStringToSpeak, int nTalkVolume)
{
    nwn_StackPushInteger(nTalkVolume);
    nwn_StackPushString(sStringToSpeak.c_str());
    nwn_ExecuteCommand(221, 2);
}

location GetSpellTargetLocation()
{
    nwn_ExecuteCommand(222, 0);
    CScriptLocation* l = (CScriptLocation*)nwn_StackPopEngineStructure(ENGINE_STRUCTURE_LOCATION);
    location loc { { l->X, l->Y, l->Z },
        { l->OrientationX, l->OrientationY, l->OrientationZ },
        l->AreaID
    };
    free(l);
    return loc;

}

vec3 GetPositionFromLocation(const location& lLocation)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lLocation);
    nwn_ExecuteCommand(223, 1);
    Vector *v = nwn_StackPopVector();
    vec3 res = { v->X, v->Y, v->Z };
    free(v);
    return res;
}

object GetAreaFromLocation(const location& lLocation)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lLocation);
    nwn_ExecuteCommand(224, 1);
    return nwn_StackPopObject();
}

float GetFacingFromLocation(const location& lLocation)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lLocation);
    nwn_ExecuteCommand(225, 1);
    return nwn_StackPopFloat();
}

object GetNearestCreatureToLocation(int nFirstCriteriaType, int nFirstCriteriaValue,
                                    const location& lLocation, int nNth, int nSecondCriteriaType,
                                    int nSecondCriteriaValue, int nThirdCriteriaType,
                                    int nThirdCriteriaValue)
{
    nwn_StackPushInteger(nThirdCriteriaValue);
    nwn_StackPushInteger(nThirdCriteriaType);
    nwn_StackPushInteger(nSecondCriteriaValue);
    nwn_StackPushInteger(nSecondCriteriaType);
    nwn_StackPushInteger(nNth);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lLocation);
    nwn_StackPushInteger(nFirstCriteriaValue);
    nwn_StackPushInteger(nFirstCriteriaType);
    nwn_ExecuteCommand(226, 8);
    return nwn_StackPopObject();
}

object GetNearestObject(int nObjectType, object oTarget, int nNth)
{
    nwn_StackPushInteger(nNth);
    nwn_StackPushObject(oTarget);
    nwn_StackPushInteger(nObjectType);
    nwn_ExecuteCommand(227, 3);
    return nwn_StackPopObject();
}

object GetNearestObjectToLocation(int nObjectType, const location& lLocation, int nNth)
{
    nwn_StackPushInteger(nNth);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lLocation);
    nwn_StackPushInteger(nObjectType);
    nwn_ExecuteCommand(228, 3);
    return nwn_StackPopObject();
}

object GetNearestObjectByTag(const string& sTag, object oTarget, int nNth)
{
    nwn_StackPushInteger(nNth);
    nwn_StackPushObject(oTarget);
    nwn_StackPushString(sTag.c_str());
    nwn_ExecuteCommand(229, 3);
    return nwn_StackPopObject();
}

float IntToFloat(int nInteger)
{
    nwn_StackPushInteger(nInteger);
    nwn_ExecuteCommand(230, 1);
    return nwn_StackPopFloat();
}

int FloatToInt(float fFloat)
{
    nwn_StackPushFloat(fFloat);
    nwn_ExecuteCommand(231, 1);
    return nwn_StackPopInteger();
}

int StringToInt(const string& sNumber)
{
    nwn_StackPushString(sNumber.c_str());
    nwn_ExecuteCommand(232, 1);
    return nwn_StackPopInteger();
}

float StringToFloat(const string& sNumber)
{
    nwn_StackPushString(sNumber.c_str());
    nwn_ExecuteCommand(233, 1);
    return nwn_StackPopFloat();
}

void ActionCastSpellAtLocation(int nSpell, const location& lTargetLocation, int nMetaMagic,
                               int bCheat,
                               int nProjectilePathType, int bInstantSpell)
{
    nwn_StackPushInteger(bInstantSpell);
    nwn_StackPushInteger(nProjectilePathType);
    nwn_StackPushInteger(bCheat);
    nwn_StackPushInteger(nMetaMagic);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lTargetLocation);
    nwn_StackPushInteger(nSpell);
    nwn_ExecuteCommand(234, 6);
}

int GetIsEnemy(object oTarget, object oSource)
{
    nwn_StackPushObject(oSource);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(235, 2);
    return nwn_StackPopInteger();
}

int GetIsFriend(object oTarget, object oSource)
{
    nwn_StackPushObject(oSource);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(236, 2);
    return nwn_StackPopInteger();
}

int GetIsNeutral(object oTarget, object oSource)
{
    nwn_StackPushObject(oSource);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(237, 2);
    return nwn_StackPopInteger();
}

object GetPCSpeaker()
{
    nwn_ExecuteCommand(238, 0);
    return nwn_StackPopObject();
}

string GetStringByStrRef(int nStrRef, int nGender)
{
    nwn_StackPushInteger(nGender);
    nwn_StackPushInteger(nStrRef);
    nwn_ExecuteCommand(239, 2);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

void ActionSpeakStringByStrRef(int nStrRef, int nTalkVolume)
{
    nwn_StackPushInteger(nTalkVolume);
    nwn_StackPushInteger(nStrRef);
    nwn_ExecuteCommand(240, 2);
}

void DestroyObject(object oDestroy, float fDelay)
{
    nwn_StackPushFloat(fDelay);
    nwn_StackPushObject(oDestroy);
    nwn_ExecuteCommand(241, 2);
}

object GetModule()
{
    nwn_ExecuteCommand(242, 0);
    return nwn_StackPopObject();
}

object CreateObject(int nObjectType, const string& sTemplate, const location& lLocation,
                    int bUseAppearAnimation, const string& sNewTag)
{
    nwn_StackPushString(sNewTag.c_str());
    nwn_StackPushInteger(bUseAppearAnimation);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lLocation);
    nwn_StackPushString(sTemplate.c_str());
    nwn_StackPushInteger(nObjectType);
    nwn_ExecuteCommand(243, 5);
    return nwn_StackPopObject();
}

event EventSpellCastAt(object oCaster, int nSpell, int bHarmful)
{
    nwn_StackPushInteger(bHarmful);
    nwn_StackPushInteger(nSpell);
    nwn_StackPushObject(oCaster);
    nwn_ExecuteCommand(244, 3);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EVENT);
}

object GetLastSpellCaster()
{
    nwn_ExecuteCommand(245, 0);
    return nwn_StackPopObject();
}

int GetLastSpell()
{
    nwn_ExecuteCommand(246, 0);
    return nwn_StackPopInteger();
}

int GetUserDefinedEventNumber()
{
    nwn_ExecuteCommand(247, 0);
    return nwn_StackPopInteger();
}

int GetSpellId()
{
    nwn_ExecuteCommand(248, 0);
    return nwn_StackPopInteger();
}

string RandomName(int nNameType)
{
    nwn_StackPushInteger(nNameType);
    nwn_ExecuteCommand(249, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

effect EffectPoison(int nPoisonType)
{
    nwn_StackPushInteger(nPoisonType);
    nwn_ExecuteCommand(250, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectDisease(int nDiseaseType)
{
    nwn_StackPushInteger(nDiseaseType);
    nwn_ExecuteCommand(251, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectSilence()
{
    nwn_ExecuteCommand(252, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

string GetName(object oObject, int bOriginalName)
{
    nwn_StackPushInteger(bOriginalName);
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(253, 2);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

object GetLastSpeaker()
{
    nwn_ExecuteCommand(254, 0);
    return nwn_StackPopObject();
}

int BeginConversation(const string& sResRef, object oObjectToDialog)
{
    nwn_StackPushObject(oObjectToDialog);
    nwn_StackPushString(sResRef.c_str());
    nwn_ExecuteCommand(255, 2);
    return nwn_StackPopInteger();
}

object GetLastPerceived()
{
    nwn_ExecuteCommand(256, 0);
    return nwn_StackPopObject();
}

int GetLastPerceptionHeard()
{
    nwn_ExecuteCommand(257, 0);
    return nwn_StackPopInteger();
}

int GetLastPerceptionInaudible()
{
    nwn_ExecuteCommand(258, 0);
    return nwn_StackPopInteger();
}

int GetLastPerceptionSeen()
{
    nwn_ExecuteCommand(259, 0);
    return nwn_StackPopInteger();
}

object GetLastClosedBy()
{
    nwn_ExecuteCommand(260, 0);
    return nwn_StackPopObject();
}

int GetLastPerceptionVanished()
{
    nwn_ExecuteCommand(261, 0);
    return nwn_StackPopInteger();
}

object GetFirstInPersistentObject(object oPersistentObject, int nResidentObjectType,
                                  int nPersistentZone)
{
    nwn_StackPushInteger(nPersistentZone);
    nwn_StackPushInteger(nResidentObjectType);
    nwn_StackPushObject(oPersistentObject);
    nwn_ExecuteCommand(262, 3);
    return nwn_StackPopObject();
}

object GetNextInPersistentObject(object oPersistentObject, int nResidentObjectType,
                                 int nPersistentZone)
{
    nwn_StackPushInteger(nPersistentZone);
    nwn_StackPushInteger(nResidentObjectType);
    nwn_StackPushObject(oPersistentObject);
    nwn_ExecuteCommand(263, 3);
    return nwn_StackPopObject();
}

object GetAreaOfEffectCreator(object oAreaOfEffectObject)
{
    nwn_StackPushObject(oAreaOfEffectObject);
    nwn_ExecuteCommand(264, 1);
    return nwn_StackPopObject();
}

void DeleteLocalInt(object oObject, const string& sVarName)
{
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(265, 2);
}

void DeleteLocalFloat(object oObject, const string& sVarName)
{
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(266, 2);
}

void DeleteLocalString(object oObject, const string& sVarName)
{
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(267, 2);
}

void DeleteLocalObject(object oObject, const string& sVarName)
{
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(268, 2);
}

void DeleteLocalLocation(object oObject, string sVarName)
{
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(269, 2);
}

effect EffectHaste()
{
    nwn_ExecuteCommand(270, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectSlow()
{
    nwn_ExecuteCommand(271, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

string ObjectToString(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(272, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

effect EffectImmunity(int nImmunityType)
{
    nwn_StackPushInteger(nImmunityType);
    nwn_ExecuteCommand(273, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

int GetIsImmune(object oCreature, int nImmunityType, object oVersus)
{
    nwn_StackPushObject(oVersus);
    nwn_StackPushInteger(nImmunityType);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(274, 3);
    return nwn_StackPopInteger();
}

effect EffectDamageImmunityIncrease(int nDamageType, int nPercentImmunity)
{
    nwn_StackPushInteger(nPercentImmunity);
    nwn_StackPushInteger(nDamageType);
    nwn_ExecuteCommand(275, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

int GetEncounterActive(object oEncounter)
{
    nwn_StackPushObject(oEncounter);
    nwn_ExecuteCommand(276, 1);
    return nwn_StackPopInteger();
}

void SetEncounterActive(int nNewValue, object oEncounter)
{
    nwn_StackPushObject(oEncounter);
    nwn_StackPushInteger(nNewValue);
    nwn_ExecuteCommand(277, 2);
}

int GetEncounterSpawnsMax(object oEncounter)
{
    nwn_StackPushObject(oEncounter);
    nwn_ExecuteCommand(278, 1);
    return nwn_StackPopInteger();
}

void SetEncounterSpawnsMax(int nNewValue, object oEncounter)
{
    nwn_StackPushObject(oEncounter);
    nwn_StackPushInteger(nNewValue);
    nwn_ExecuteCommand(279, 2);
}

int GetEncounterSpawnsCurrent(object oEncounter)
{
    nwn_StackPushObject(oEncounter);
    nwn_ExecuteCommand(280, 1);
    return nwn_StackPopInteger();
}

void SetEncounterSpawnsCurrent(int nNewValue, object oEncounter)
{
    nwn_StackPushObject(oEncounter);
    nwn_StackPushInteger(nNewValue);
    nwn_ExecuteCommand(281, 2);
}

object GetModuleItemAcquired()
{
    nwn_ExecuteCommand(282, 0);
    return nwn_StackPopObject();
}

object GetModuleItemAcquiredFrom()
{
    nwn_ExecuteCommand(283, 0);
    return nwn_StackPopObject();
}

void SetCustomToken(int nCustomTokenNumber, const string& sTokenValue)
{
    nwn_StackPushString(sTokenValue.c_str());
    nwn_StackPushInteger(nCustomTokenNumber);
    nwn_ExecuteCommand(284, 2);
}

int GetHasFeat(int nFeat, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nFeat);
    nwn_ExecuteCommand(285, 2);
    return nwn_StackPopInteger();
}

int GetHasSkill(int nSkill, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nSkill);
    nwn_ExecuteCommand(286, 2);
    return nwn_StackPopInteger();
}

void ActionUseFeat(int nFeat, object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_StackPushInteger(nFeat);
    nwn_ExecuteCommand(287, 2);
}

void ActionUseSkill(int nSkill, object oTarget, int nSubSkill, object oItemUsed)
{
    nwn_StackPushObject(oItemUsed);
    nwn_StackPushInteger(nSubSkill);
    nwn_StackPushObject(oTarget);
    nwn_StackPushInteger(nSkill);
    nwn_ExecuteCommand(288, 4);
}

int GetObjectSeen(object oTarget, object oSource)
{
    nwn_StackPushObject(oSource);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(289, 2);
    return nwn_StackPopInteger();
}

int GetObjectHeard(object oTarget, object oSource)
{
    nwn_StackPushObject(oSource);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(290, 2);
    return nwn_StackPopInteger();
}

object GetLastPlayerDied()
{
    nwn_ExecuteCommand(291, 0);
    return nwn_StackPopObject();
}

object GetModuleItemLost()
{
    nwn_ExecuteCommand(292, 0);
    return nwn_StackPopObject();
}

object GetModuleItemLostBy()
{
    nwn_ExecuteCommand(293, 0);
    return nwn_StackPopObject();
}
/*
int ActionDoCommand(VALUE aActionToDo)
{
  //ERROR: Undefined variable type: action
  nwn_ExecuteCommand(294, 1);

}
*/
event EventConversation()
{
    nwn_ExecuteCommand(295, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EVENT);
}

void SetEncounterDifficulty(int nEncounterDifficulty, object oEncounter)
{
    nwn_StackPushObject(oEncounter);
    nwn_StackPushInteger(nEncounterDifficulty);
    nwn_ExecuteCommand(296, 2);
}

int GetEncounterDifficulty(object oEncounter)
{
    nwn_StackPushObject(oEncounter);
    nwn_ExecuteCommand(297, 1);
    return nwn_StackPopInteger();
}

float GetDistanceBetweenLocations(const location& lLocationA, const location& lLocationB)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lLocationB);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lLocationA);
    nwn_ExecuteCommand(298, 2);
    return nwn_StackPopFloat();
}

int GetReflexAdjustedDamage(int nDamage, object oTarget, int nDC, int nSaveType,
                            object oSaveVersus)
{
    nwn_StackPushObject(oSaveVersus);
    nwn_StackPushInteger(nSaveType);
    nwn_StackPushInteger(nDC);
    nwn_StackPushObject(oTarget);
    nwn_StackPushInteger(nDamage);
    nwn_ExecuteCommand(299, 5);
    return nwn_StackPopInteger();
}

void PlayAnimation(int nAnimation, float fSpeed, float fSeconds)
{
    nwn_StackPushFloat(fSeconds);
    nwn_StackPushFloat(fSpeed);
    nwn_StackPushInteger(nAnimation);
    nwn_ExecuteCommand(300, 3);
}

talent TalentSpell(int nSpell)
{
    nwn_StackPushInteger(nSpell);
    nwn_ExecuteCommand(301, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_TALENT);
}

talent TalentFeat(int nFeat)
{
    nwn_StackPushInteger(nFeat);
    nwn_ExecuteCommand(302, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_TALENT);
}

talent TalentSkill(int nSkill)
{
    nwn_StackPushInteger(nSkill);
    nwn_ExecuteCommand(303, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_TALENT);
}

int GetHasSpellEffect(int nSpell, object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_StackPushInteger(nSpell);
    nwn_ExecuteCommand(304, 2);
    return nwn_StackPopInteger();
}

int GetEffectSpellId(const effect& eSpellEffect)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, eSpellEffect.structure);
    nwn_ExecuteCommand(305, 1);
    return nwn_StackPopInteger();
}

int GetCreatureHasTalent(const talent& tTalent, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_TALENT, tTalent.structure);
    nwn_ExecuteCommand(306, 2);
    return nwn_StackPopInteger();
}

talent GetCreatureTalentRandom(int nCategory, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nCategory);
    nwn_ExecuteCommand(307, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_TALENT);
}

talent GetCreatureTalentBest(int nCategory, int nCRMax, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nCRMax);
    nwn_StackPushInteger(nCategory);
    nwn_ExecuteCommand(308, 3);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_TALENT);
}

void ActionUseTalentOnObject(const talent& tChosenTalent, object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_TALENT, tChosenTalent.structure);
    nwn_ExecuteCommand(309, 2);
}

void ActionUseTalentAtLocation(const talent& tChosenTalent, const location& lTargetLocation)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lTargetLocation);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_TALENT, tChosenTalent.structure);
    nwn_ExecuteCommand(310, 2);
}

int GetGoldPieceValue(object oItem)
{
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(311, 1);
    return nwn_StackPopInteger();
}

int GetIsPlayableRacialType(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(312, 1);
    return nwn_StackPopInteger();
}

void JumpToLocation(const location& lDestination)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lDestination);
    nwn_ExecuteCommand(313, 1);
}

effect EffectTemporaryHitpoints(int nHitPoints)
{
    nwn_StackPushInteger(nHitPoints);
    nwn_ExecuteCommand(314, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

int GetSkillRank(int nSkill, object oTarget, int nBaseSkillRank)
{
    nwn_StackPushInteger(nBaseSkillRank);
    nwn_StackPushObject(oTarget);
    nwn_StackPushInteger(nSkill);
    nwn_ExecuteCommand(315, 3);
    return nwn_StackPopInteger();
}

object GetAttackTarget(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(316, 1);
    return nwn_StackPopObject();
}

int GetLastAttackType(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(317, 1);
    return nwn_StackPopInteger();
}

int GetLastAttackMode(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(318, 1);
    return nwn_StackPopInteger();
}

object GetMaster(object oAssociate)
{
    nwn_StackPushObject(oAssociate);
    nwn_ExecuteCommand(319, 1);
    return nwn_StackPopObject();
}

int GetIsInCombat(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(320, 1);
    return nwn_StackPopInteger();
}

int GetLastAssociateCommand(object oAssociate)
{
    nwn_StackPushObject(oAssociate);
    nwn_ExecuteCommand(321, 1);
    return nwn_StackPopInteger();
}

void GiveGoldToCreature(object oCreature, int nGP)
{
    nwn_StackPushInteger(nGP);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(322, 2);
}

void SetIsDestroyable(int bDestroyable, int bRaiseable, int bSelectableWhenDead)
{
    nwn_StackPushInteger(bSelectableWhenDead);
    nwn_StackPushInteger(bRaiseable);
    nwn_StackPushInteger(bDestroyable);
    nwn_ExecuteCommand(323, 3);
}

void SetLocked(object oTarget, int bLocked)
{
    nwn_StackPushInteger(bLocked);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(324, 2);
}

int GetLocked(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(325, 1);
    return nwn_StackPopInteger();
}

object GetClickingObject()
{
    nwn_ExecuteCommand(326, 0);
    return nwn_StackPopObject();
}

void SetAssociateListenPatterns(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(327, 1);
}

object GetLastWeaponUsed(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(328, 1);
    return nwn_StackPopObject();
}

void ActionInteractObject(object oPlaceable)
{
    nwn_StackPushObject(oPlaceable);
    nwn_ExecuteCommand(329, 1);
}

object GetLastUsedBy()
{
    nwn_ExecuteCommand(330, 0);
    return nwn_StackPopObject();
}

int GetAbilityModifier(int nAbility, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nAbility);
    nwn_ExecuteCommand(331, 2);
    return nwn_StackPopInteger();
}

int GetIdentified(object oItem)
{
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(332, 1);
    return nwn_StackPopInteger();
}

void SetIdentified(object oItem, int bIdentified)
{
    nwn_StackPushInteger(bIdentified);
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(333, 2);
}

void SummonAnimalCompanion(object oMaster)
{
    nwn_StackPushObject(oMaster);
    nwn_ExecuteCommand(334, 1);
}

void SummonFamiliar(object oMaster)
{
    nwn_StackPushObject(oMaster);
    nwn_ExecuteCommand(335, 1);
}

object GetBlockingDoor()
{
    nwn_ExecuteCommand(336, 0);
    return nwn_StackPopObject();
}

int GetIsDoorActionPossible(object oTargetDoor, int nDoorAction)
{
    nwn_StackPushInteger(nDoorAction);
    nwn_StackPushObject(oTargetDoor);
    nwn_ExecuteCommand(337, 2);
    return nwn_StackPopInteger();
}

void DoDoorAction(object oTargetDoor, int nDoorAction)
{
    nwn_StackPushInteger(nDoorAction);
    nwn_StackPushObject(oTargetDoor);
    nwn_ExecuteCommand(338, 2);
}

object GetFirstItemInInventory(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(339, 1);
    return nwn_StackPopObject();
}

object GetNextItemInInventory(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(340, 1);
    return nwn_StackPopObject();
}

int GetClassByPosition(int nClassPosition, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nClassPosition);
    nwn_ExecuteCommand(341, 2);
    return nwn_StackPopInteger();
}

int GetLevelByPosition(int nClassPosition, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nClassPosition);
    nwn_ExecuteCommand(342, 2);
    return nwn_StackPopInteger();
}

int GetLevelByClass(int nClassType, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nClassType);
    nwn_ExecuteCommand(343, 2);
    return nwn_StackPopInteger();
}

int GetDamageDealtByType(int nDamageType)
{
    nwn_StackPushInteger(nDamageType);
    nwn_ExecuteCommand(344, 1);
    return nwn_StackPopInteger();
}

int GetTotalDamageDealt()
{
    nwn_ExecuteCommand(345, 0);
    return nwn_StackPopInteger();
}

object GetLastDamager(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(346, 1);
    return nwn_StackPopObject();
}

object GetLastDisarmed()
{
    nwn_ExecuteCommand(347, 0);
    return nwn_StackPopObject();
}

object GetLastDisturbed()
{
    nwn_ExecuteCommand(348, 0);
    return nwn_StackPopObject();
}

object GetLastLocked()
{
    nwn_ExecuteCommand(349, 0);
    return nwn_StackPopObject();
}

object GetLastUnlocked()
{
    nwn_ExecuteCommand(350, 0);
    return nwn_StackPopObject();
}

effect EffectSkillIncrease(int nSkill, int nValue)
{
    nwn_StackPushInteger(nValue);
    nwn_StackPushInteger(nSkill);
    nwn_ExecuteCommand(351, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

int GetInventoryDisturbType()
{
    nwn_ExecuteCommand(352, 0);
    return nwn_StackPopInteger();
}

object GetInventoryDisturbItem()
{
    nwn_ExecuteCommand(353, 0);
    return nwn_StackPopObject();
}

object GetHenchman(object oMaster, int nNth)
{
    nwn_StackPushInteger(nNth);
    nwn_StackPushObject(oMaster);
    nwn_ExecuteCommand(354, 2);
    return nwn_StackPopObject();
}

effect VersusAlignmentEffect(const effect& eEffect, int nLawChaos, int nGoodEvil)
{
    nwn_StackPushInteger(nGoodEvil);
    nwn_StackPushInteger(nLawChaos);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, eEffect.structure);
    nwn_ExecuteCommand(355, 3);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect VersusRacialTypeEffect(const effect& eEffect, int nRacialType)
{
    nwn_StackPushInteger(nRacialType);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, eEffect.structure);
    nwn_ExecuteCommand(356, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect VersusTrapEffect(const effect& eEffect)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, eEffect.structure);
    nwn_ExecuteCommand(357, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

int GetGender(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(358, 1);
    return nwn_StackPopInteger();
}

int GetIsTalentValid(const talent& tTalent)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_TALENT, tTalent.structure);
    nwn_ExecuteCommand(359, 1);
    return nwn_StackPopInteger();
}

void ActionMoveAwayFromLocation(const location& lMoveAwayFrom, int bRun, float fMoveAwayRange)
{
    nwn_StackPushFloat(fMoveAwayRange);
    nwn_StackPushInteger(bRun);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lMoveAwayFrom);
    nwn_ExecuteCommand(360, 3);
}

object GetAttemptedAttackTarget()
{
    nwn_ExecuteCommand(361, 0);
    return nwn_StackPopObject();
}

int GetTypeFromTalent(const talent& tTalent)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_TALENT, tTalent.structure);
    nwn_ExecuteCommand(362, 1);
    return nwn_StackPopInteger();
}

int GetIdFromTalent(const talent& tTalent)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_TALENT, tTalent.structure);
    nwn_ExecuteCommand(363, 1);
    return nwn_StackPopInteger();
}

object GetAssociate(int nAssociateType, object oMaster, int nTh)
{
    nwn_StackPushInteger(nTh);
    nwn_StackPushObject(oMaster);
    nwn_StackPushInteger(nAssociateType);
    nwn_ExecuteCommand(364, 3);
    return nwn_StackPopObject();
}

void AddHenchman(object oMaster, object oHenchman)
{
    nwn_StackPushObject(oHenchman);
    nwn_StackPushObject(oMaster);
    nwn_ExecuteCommand(365, 2);
}

void RemoveHenchman(object oMaster, object oHenchman)
{
    nwn_StackPushObject(oHenchman);
    nwn_StackPushObject(oMaster);
    nwn_ExecuteCommand(366, 2);
}

void AddJournalQuestEntry(const string& szPlotID, int nState, object oCreature,
                          int bAllPartyMembers,
                          int bAllPlayers, int bAllowOverrideHigher)
{
    nwn_StackPushInteger(bAllowOverrideHigher);
    nwn_StackPushInteger(bAllPlayers);
    nwn_StackPushInteger(bAllPartyMembers);
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nState);
    nwn_StackPushString(szPlotID.c_str());
    nwn_ExecuteCommand(367, 6);
}

void RemoveJournalQuestEntry(const string& szPlotID, object oCreature, int bAllPartyMembers,
                             int bAllPlayers)
{
    nwn_StackPushInteger(bAllPlayers);
    nwn_StackPushInteger(bAllPartyMembers);
    nwn_StackPushObject(oCreature);
    nwn_StackPushString(szPlotID.c_str());
    nwn_ExecuteCommand(368, 4);
}

string GetPCPublicCDKey(object oPlayer, int nSinglePlayerCDKey)
{
    nwn_StackPushInteger(nSinglePlayerCDKey);
    nwn_StackPushObject(oPlayer);
    nwn_ExecuteCommand(369, 2);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

string GetPCIPAddress(object oPlayer)
{
    nwn_StackPushObject(oPlayer);
    nwn_ExecuteCommand(370, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

string GetPCPlayerName(object oPlayer)
{
    nwn_StackPushObject(oPlayer);
    nwn_ExecuteCommand(371, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

void SetPCLike(object oPlayer, object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_StackPushObject(oPlayer);
    nwn_ExecuteCommand(372, 2);
}

void SetPCDislike(object oPlayer, object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_StackPushObject(oPlayer);
    nwn_ExecuteCommand(373, 2);
}

void SendMessageToPC(object oPlayer, const string& sMessage)
{
    nwn_StackPushString(sMessage.c_str());
    nwn_StackPushObject(oPlayer);
    nwn_ExecuteCommand(374, 2);
}

object GetAttemptedSpellTarget()
{
    nwn_ExecuteCommand(375, 0);
    return nwn_StackPopObject();
}

object GetLastOpenedBy()
{
    nwn_ExecuteCommand(376, 0);
    return nwn_StackPopObject();
}

int GetHasSpell(int nSpell, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nSpell);
    nwn_ExecuteCommand(377, 2);
    return nwn_StackPopInteger();
}

void OpenStore(object oStore, object oPC, int nBonusMarkUp, int nBonusMarkDown)
{
    nwn_StackPushInteger(nBonusMarkDown);
    nwn_StackPushInteger(nBonusMarkUp);
    nwn_StackPushObject(oPC);
    nwn_StackPushObject(oStore);
    nwn_ExecuteCommand(378, 4);
}

effect EffectTurned()
{
    nwn_ExecuteCommand(379, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

object GetFirstFactionMember(object oMemberOfFaction, int bPCOnly)
{
    nwn_StackPushInteger(bPCOnly);
    nwn_StackPushObject(oMemberOfFaction);
    nwn_ExecuteCommand(380, 2);
    return nwn_StackPopObject();
}

object GetNextFactionMember(object oMemberOfFaction, int bPCOnly)
{
    nwn_StackPushInteger(bPCOnly);
    nwn_StackPushObject(oMemberOfFaction);
    nwn_ExecuteCommand(381, 2);
    return nwn_StackPopObject();
}

void ActionForceMoveToLocation(const location& lDestination, int bRun, float fTimeout)
{
    nwn_StackPushFloat(fTimeout);
    nwn_StackPushInteger(bRun);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lDestination);
    nwn_ExecuteCommand(382, 3);
}

void ActionForceMoveToObject(object oMoveTo, int bRun, float fRange, float fTimeout)
{
    nwn_StackPushFloat(fTimeout);
    nwn_StackPushFloat(fRange);
    nwn_StackPushInteger(bRun);
    nwn_StackPushObject(oMoveTo);
    nwn_ExecuteCommand(383, 4);
}

int GetJournalQuestExperience(const string& szPlotID)
{
    nwn_StackPushString(szPlotID.c_str());
    nwn_ExecuteCommand(384, 1);
    return nwn_StackPopInteger();
}

void JumpToObject(object oToJumpTo, int nWalkStraightLineToPoint)
{
    nwn_StackPushInteger(nWalkStraightLineToPoint);
    nwn_StackPushObject(oToJumpTo);
    nwn_ExecuteCommand(385, 2);
}

void SetMapPinEnabled(object oMapPin, int nEnabled)
{
    nwn_StackPushInteger(nEnabled);
    nwn_StackPushObject(oMapPin);
    nwn_ExecuteCommand(386, 2);
}

effect EffectHitPointChangeWhenDying(float fHitPointChangePerRound)
{
    nwn_StackPushFloat(fHitPointChangePerRound);
    nwn_ExecuteCommand(387, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

void PopUpGUIPanel(object oPC, int nGUIPanel)
{
    nwn_StackPushInteger(nGUIPanel);
    nwn_StackPushObject(oPC);
    nwn_ExecuteCommand(388, 2);
}

void ClearPersonalReputation(object oTarget, object oSource)
{
    nwn_StackPushObject(oSource);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(389, 2);
}

void SetIsTemporaryFriend(object oTarget, object oSource, int bDecays, float fDurationInSeconds)
{
    nwn_StackPushFloat(fDurationInSeconds);
    nwn_StackPushInteger(bDecays);
    nwn_StackPushObject(oSource);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(390, 4);
}

void SetIsTemporaryEnemy(object oTarget, object oSource, int bDecays, float fDurationInSeconds)
{
    nwn_StackPushFloat(fDurationInSeconds);
    nwn_StackPushInteger(bDecays);
    nwn_StackPushObject(oSource);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(391, 4);
}

void SetIsTemporaryNeutral(object oTarget, object oSource, int bDecays, float fDurationInSeconds)
{
    nwn_StackPushFloat(fDurationInSeconds);
    nwn_StackPushInteger(bDecays);
    nwn_StackPushObject(oSource);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(392, 4);
}

void GiveXPToCreature(object oCreature, int nXpAmount)
{
    nwn_StackPushInteger(nXpAmount);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(393, 2);
}

void SetXP(object oCreature, int nXpAmount)
{
    nwn_StackPushInteger(nXpAmount);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(394, 2);
}

int GetXP(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(395, 1);
    return nwn_StackPopInteger();
}

string IntToHexString(int nInteger)
{
    nwn_StackPushInteger(nInteger);
    nwn_ExecuteCommand(396, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

int GetBaseItemType(object oItem)
{
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(397, 1);
    return nwn_StackPopInteger();
}

int GetItemHasItemProperty(object oItem, int nProperty)
{
    nwn_StackPushInteger(nProperty);
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(398, 2);
    return nwn_StackPopInteger();
}

void ActionEquipMostDamagingMelee(object oVersus, int bOffHand)
{
    nwn_StackPushInteger(bOffHand);
    nwn_StackPushObject(oVersus);
    nwn_ExecuteCommand(399, 2);
}

void ActionEquipMostDamagingRanged(object oVersus)
{
    nwn_StackPushObject(oVersus);
    nwn_ExecuteCommand(400, 1);
}

int GetItemACValue(object oItem)
{
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(401, 1);
    return nwn_StackPopInteger();
}

void ActionRest(int bCreatureToEnemyLineOfSightCheck)
{
    nwn_StackPushInteger(bCreatureToEnemyLineOfSightCheck);
    nwn_ExecuteCommand(402, 1);
}

void ExploreAreaForPlayer(object oArea, object oPlayer, int bExplored)
{
    nwn_StackPushInteger(bExplored);
    nwn_StackPushObject(oPlayer);
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(403, 3);
}

void ActionEquipMostEffectiveArmor()
{
    nwn_ExecuteCommand(404, 0);
}

int GetIsDay()
{
    nwn_ExecuteCommand(405, 0);
    return nwn_StackPopInteger();
}

int GetIsNight()
{
    nwn_ExecuteCommand(406, 0);
    return nwn_StackPopInteger();
}

int GetIsDawn()
{
    nwn_ExecuteCommand(407, 0);
    return nwn_StackPopInteger();
}

int GetIsDusk()
{
    nwn_ExecuteCommand(408, 0);
    return nwn_StackPopInteger();
}

int GetIsEncounterCreature(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(409, 1);
    return nwn_StackPopInteger();
}

object GetLastPlayerDying()
{
    nwn_ExecuteCommand(410, 0);
    return nwn_StackPopObject();
}

location GetStartingLocation()
{
    nwn_ExecuteCommand(411, 0);
    CScriptLocation* l = (CScriptLocation*)nwn_StackPopEngineStructure(ENGINE_STRUCTURE_LOCATION);
    location loc { { l->X, l->Y, l->Z },
        { l->OrientationX, l->OrientationY, l->OrientationZ },
        l->AreaID
    };
    free(l);
    return loc;

}

void ChangeToStandardFaction(object oCreatureToChange, int nStandardFaction)
{
    nwn_StackPushInteger(nStandardFaction);
    nwn_StackPushObject(oCreatureToChange);
    nwn_ExecuteCommand(412, 2);
}

void SoundObjectPlay(object oSound)
{
    nwn_StackPushObject(oSound);
    nwn_ExecuteCommand(413, 1);
}

void SoundObjectStop(object oSound)
{
    nwn_StackPushObject(oSound);
    nwn_ExecuteCommand(414, 1);
}

void SoundObjectSetVolume(object oSound, int nVolume)
{
    nwn_StackPushInteger(nVolume);
    nwn_StackPushObject(oSound);
    nwn_ExecuteCommand(415, 2);
}

void SoundObjectSetPosition(object oSound, vec3 vPosition)
{
    nwn_StackPushVector((Vector*)&vPosition);
    nwn_StackPushObject(oSound);
    nwn_ExecuteCommand(416, 2);
}

void SpeakOneLinerConversation(const string& sDialogResRef, object oTokenTarget)
{
    nwn_StackPushObject(oTokenTarget);
    nwn_StackPushString(sDialogResRef.c_str());
    nwn_ExecuteCommand(417, 2);
}

int GetGold(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(418, 1);
    return nwn_StackPopInteger();
}

object GetLastRespawnButtonPresser()
{
    nwn_ExecuteCommand(419, 0);
    return nwn_StackPopObject();
}

int GetIsDM(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(420, 1);
    return nwn_StackPopInteger();
}

void PlayVoiceChat(int nVoiceChatID, object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_StackPushInteger(nVoiceChatID);
    nwn_ExecuteCommand(421, 2);
}

int GetIsWeaponEffective(object oVersus, int bOffHand)
{
    nwn_StackPushInteger(bOffHand);
    nwn_StackPushObject(oVersus);
    nwn_ExecuteCommand(422, 2);
    return nwn_StackPopInteger();
}

int GetLastSpellHarmful()
{
    nwn_ExecuteCommand(423, 0);
    return nwn_StackPopInteger();
}

event EventActivateItem(object oItem, const location& lTarget, object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lTarget);
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(424, 3);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EVENT);
}

void MusicBackgroundPlay(object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(425, 1);
}

void MusicBackgroundStop(object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(426, 1);
}

void MusicBackgroundSetDelay(object oArea, int nDelay)
{
    nwn_StackPushInteger(nDelay);
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(427, 2);
}

void MusicBackgroundChangeDay(object oArea, int nTrack)
{
    nwn_StackPushInteger(nTrack);
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(428, 2);
}

void MusicBackgroundChangeNight(object oArea, int nTrack)
{
    nwn_StackPushInteger(nTrack);
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(429, 2);
}

void MusicBattlePlay(object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(430, 1);
}

void MusicBattleStop(object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(431, 1);
}

void MusicBattleChange(object oArea, int nTrack)
{
    nwn_StackPushInteger(nTrack);
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(432, 2);
}

void AmbientSoundPlay(object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(433, 1);
}

void AmbientSoundStop(object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(434, 1);
}

void AmbientSoundChangeDay(object oArea, int nTrack)
{
    nwn_StackPushInteger(nTrack);
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(435, 2);
}

void AmbientSoundChangeNight(object oArea, int nTrack)
{
    nwn_StackPushInteger(nTrack);
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(436, 2);
}

object GetLastKiller()
{
    nwn_ExecuteCommand(437, 0);
    return nwn_StackPopObject();
}

object GetSpellCastItem()
{
    nwn_ExecuteCommand(438, 0);
    return nwn_StackPopObject();
}

object GetItemActivated()
{
    nwn_ExecuteCommand(439, 0);
    return nwn_StackPopObject();
}

object GetItemActivator()
{
    nwn_ExecuteCommand(440, 0);
    return nwn_StackPopObject();
}

location GetItemActivatedTargetLocation()
{
    nwn_ExecuteCommand(441, 0);
    CScriptLocation* l = (CScriptLocation*)nwn_StackPopEngineStructure(ENGINE_STRUCTURE_LOCATION);
    location loc {
        { l->X, l->Y, l->Z },
        { l->OrientationX, l->OrientationY, l->OrientationZ },
        l->AreaID
    };
    free(l);
    return loc;

}

object GetItemActivatedTarget()
{
    nwn_ExecuteCommand(442, 0);
    return nwn_StackPopObject();
}

int GetIsOpen(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(443, 1);
    return nwn_StackPopInteger();
}

void TakeGoldFromCreature(int nAmount, object oCreatureToTakeFrom, int bDestroy)
{
    nwn_StackPushInteger(bDestroy);
    nwn_StackPushObject(oCreatureToTakeFrom);
    nwn_StackPushInteger(nAmount);
    nwn_ExecuteCommand(444, 3);
}

int IsInConversation(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(445, 1);
    return nwn_StackPopInteger();
}

effect EffectAbilityDecrease(int nAbility, int nModifyBy)
{
    nwn_StackPushInteger(nModifyBy);
    nwn_StackPushInteger(nAbility);
    nwn_ExecuteCommand(446, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectAttackDecrease(int nPenalty, int nModifierType)
{
    nwn_StackPushInteger(nModifierType);
    nwn_StackPushInteger(nPenalty);
    nwn_ExecuteCommand(447, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectDamageDecrease(int nPenalty, int nDamageType)
{
    nwn_StackPushInteger(nDamageType);
    nwn_StackPushInteger(nPenalty);
    nwn_ExecuteCommand(448, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectDamageImmunityDecrease(int nDamageType, int nPercentImmunity)
{
    nwn_StackPushInteger(nPercentImmunity);
    nwn_StackPushInteger(nDamageType);
    nwn_ExecuteCommand(449, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectACDecrease(int nValue, int nModifyType, int nDamageType)
{
    nwn_StackPushInteger(nDamageType);
    nwn_StackPushInteger(nModifyType);
    nwn_StackPushInteger(nValue);
    nwn_ExecuteCommand(450, 3);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectMovementSpeedDecrease(int nPercentChange)
{
    nwn_StackPushInteger(nPercentChange);
    nwn_ExecuteCommand(451, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectSavingThrowDecrease(int nSave, int nValue, int nSaveType)
{
    nwn_StackPushInteger(nSaveType);
    nwn_StackPushInteger(nValue);
    nwn_StackPushInteger(nSave);
    nwn_ExecuteCommand(452, 3);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectSkillDecrease(int nSkill, int nValue)
{
    nwn_StackPushInteger(nValue);
    nwn_StackPushInteger(nSkill);
    nwn_ExecuteCommand(453, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectSpellResistanceDecrease(int nValue)
{
    nwn_StackPushInteger(nValue);
    nwn_ExecuteCommand(454, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

int GetPlotFlag(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(455, 1);
    return nwn_StackPopInteger();
}

void SetPlotFlag(object oTarget, int nPlotFlag)
{
    nwn_StackPushInteger(nPlotFlag);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(456, 2);
}

effect EffectInvisibility(int nInvisibilityType)
{
    nwn_StackPushInteger(nInvisibilityType);
    nwn_ExecuteCommand(457, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectConcealment(int nPercentage, int nMissType)
{
    nwn_StackPushInteger(nMissType);
    nwn_StackPushInteger(nPercentage);
    nwn_ExecuteCommand(458, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectDarkness()
{
    nwn_ExecuteCommand(459, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectDispelMagicAll(int nCasterLevel)
{
    nwn_StackPushInteger(nCasterLevel);
    nwn_ExecuteCommand(460, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectUltravision()
{
    nwn_ExecuteCommand(461, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectNegativeLevel(int nNumLevels, int bHPBonus)
{
    nwn_StackPushInteger(bHPBonus);
    nwn_StackPushInteger(nNumLevels);
    nwn_ExecuteCommand(462, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectPolymorph(int nPolymorphSelection, int nLocked)
{
    nwn_StackPushInteger(nLocked);
    nwn_StackPushInteger(nPolymorphSelection);
    nwn_ExecuteCommand(463, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectSanctuary(int nDifficultyClass)
{
    nwn_StackPushInteger(nDifficultyClass);
    nwn_ExecuteCommand(464, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectTrueSeeing()
{
    nwn_ExecuteCommand(465, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectSeeInvisible()
{
    nwn_ExecuteCommand(466, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectTimeStop()
{
    nwn_ExecuteCommand(467, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectBlindness()
{
    nwn_ExecuteCommand(468, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

int GetIsReactionTypeFriendly(object oTarget, object oSource)
{
    nwn_StackPushObject(oSource);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(469, 2);
    return nwn_StackPopInteger();
}

int GetIsReactionTypeNeutral(object oTarget, object oSource)
{
    nwn_StackPushObject(oSource);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(470, 2);
    return nwn_StackPopInteger();
}

int GetIsReactionTypeHostile(object oTarget, object oSource)
{
    nwn_StackPushObject(oSource);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(471, 2);
    return nwn_StackPopInteger();
}

effect EffectSpellLevelAbsorption(int nMaxSpellLevelAbsorbed, int nTotalSpellLevelsAbsorbed,
                                  int nSpellSchool)
{
    nwn_StackPushInteger(nSpellSchool);
    nwn_StackPushInteger(nTotalSpellLevelsAbsorbed);
    nwn_StackPushInteger(nMaxSpellLevelAbsorbed);
    nwn_ExecuteCommand(472, 3);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectDispelMagicBest(int nCasterLevel)
{
    nwn_StackPushInteger(nCasterLevel);
    nwn_ExecuteCommand(473, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

void ActivatePortal(object oTarget, const string& sIPaddress, const string& sPassword,
                    const string& sWaypointTag, int bSeemless)
{
    nwn_StackPushInteger(bSeemless);
    nwn_StackPushString(sWaypointTag.c_str());
    nwn_StackPushString(sPassword.c_str());
    nwn_StackPushString(sIPaddress.c_str());
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(474, 5);
}

int GetNumStackedItems(object oItem)
{
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(475, 1);
    return nwn_StackPopInteger();
}

void SurrenderToEnemies()
{
    nwn_ExecuteCommand(476, 0);
}

effect EffectMissChance(int nPercentage, int nMissChanceType)
{
    nwn_StackPushInteger(nMissChanceType);
    nwn_StackPushInteger(nPercentage);
    nwn_ExecuteCommand(477, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

int GetTurnResistanceHD(object oUndead)
{
    nwn_StackPushObject(oUndead);
    nwn_ExecuteCommand(478, 1);
    return nwn_StackPopInteger();
}

int GetCreatureSize(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(479, 1);
    return nwn_StackPopInteger();
}

effect EffectDisappearAppear(const location& lLocation, int nAnimation)
{
    nwn_StackPushInteger(nAnimation);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lLocation);
    nwn_ExecuteCommand(480, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectDisappear(int nAnimation)
{
    nwn_StackPushInteger(nAnimation);
    nwn_ExecuteCommand(481, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectAppear(int nAnimation)
{
    nwn_StackPushInteger(nAnimation);
    nwn_ExecuteCommand(482, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

void ActionUnlockObject(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(483, 1);
}

void ActionLockObject(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(484, 1);
}

effect EffectModifyAttacks(int nAttacks)
{
    nwn_StackPushInteger(nAttacks);
    nwn_ExecuteCommand(485, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

object GetLastTrapDetected(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(486, 1);
    return nwn_StackPopObject();
}

effect EffectDamageShield(int nDamageAmount, int nRandomAmount, int nDamageType)
{
    nwn_StackPushInteger(nDamageType);
    nwn_StackPushInteger(nRandomAmount);
    nwn_StackPushInteger(nDamageAmount);
    nwn_ExecuteCommand(487, 3);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

object GetNearestTrapToObject(object oTarget, int nTrapDetected)
{
    nwn_StackPushInteger(nTrapDetected);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(488, 2);
    return nwn_StackPopObject();
}

string GetDeity(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(489, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

string GetSubRace(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(490, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

int GetFortitudeSavingThrow(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(491, 1);
    return nwn_StackPopInteger();
}

int GetWillSavingThrow(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(492, 1);
    return nwn_StackPopInteger();
}

int GetReflexSavingThrow(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(493, 1);
    return nwn_StackPopInteger();
}

float GetChallengeRating(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(494, 1);
    return nwn_StackPopFloat();
}

int GetAge(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(495, 1);
    return nwn_StackPopInteger();
}

int GetMovementRate(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(496, 1);
    return nwn_StackPopInteger();
}

int GetFamiliarCreatureType(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(497, 1);
    return nwn_StackPopInteger();
}

int GetAnimalCompanionCreatureType(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(498, 1);
    return nwn_StackPopInteger();
}

string GetFamiliarName(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(499, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

string GetAnimalCompanionName(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(500, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

void ActionCastFakeSpellAtObject(int nSpell, object oTarget, int nProjectilePathType)
{
    nwn_StackPushInteger(nProjectilePathType);
    nwn_StackPushObject(oTarget);
    nwn_StackPushInteger(nSpell);
    nwn_ExecuteCommand(501, 3);
}

void ActionCastFakeSpellAtLocation(int nSpell, const location& lTarget, int nProjectilePathType)
{
    nwn_StackPushInteger(nProjectilePathType);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lTarget);
    nwn_StackPushInteger(nSpell);
    nwn_ExecuteCommand(502, 3);
}

void RemoveSummonedAssociate(object oMaster, object oAssociate)
{
    nwn_StackPushObject(oAssociate);
    nwn_StackPushObject(oMaster);
    nwn_ExecuteCommand(503, 2);
}

void SetCameraMode(object oPlayer, int nCameraMode)
{
    nwn_StackPushInteger(nCameraMode);
    nwn_StackPushObject(oPlayer);
    nwn_ExecuteCommand(504, 2);
}

int GetIsResting(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(505, 1);
    return nwn_StackPopInteger();
}

object GetLastPCRested()
{
    nwn_ExecuteCommand(506, 0);
    return nwn_StackPopObject();
}

void SetWeather(object oTarget, int nWeather)
{
    nwn_StackPushInteger(nWeather);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(507, 2);
}

int GetLastRestEventType()
{
    nwn_ExecuteCommand(508, 0);
    return nwn_StackPopInteger();
}

void StartNewModule(const string& sModuleName)
{
    nwn_StackPushString(sModuleName.c_str());
    nwn_ExecuteCommand(509, 1);
}

effect EffectSwarm(int nLooping, const string& sCreatureTemplate1, const string& sCreatureTemplate2,
                   const string& sCreatureTemplate3, const string& sCreatureTemplate4)
{
    nwn_StackPushString(sCreatureTemplate4.c_str());
    nwn_StackPushString(sCreatureTemplate3.c_str());
    nwn_StackPushString(sCreatureTemplate2.c_str());
    nwn_StackPushString(sCreatureTemplate1.c_str());
    nwn_StackPushInteger(nLooping);
    nwn_ExecuteCommand(510, 5);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

int GetWeaponRanged(object oItem)
{
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(511, 1);
    return nwn_StackPopInteger();
}

void DoSinglePlayerAutoSave()
{
    nwn_ExecuteCommand(512, 0);
}

int GetGameDifficulty()
{
    nwn_ExecuteCommand(513, 0);
    return nwn_StackPopInteger();
}

void SetTileMainLightColor(const location& lTileLocation, int nMainLight1Color,
                           int nMainLight2Color)
{
    nwn_StackPushInteger(nMainLight2Color);
    nwn_StackPushInteger(nMainLight1Color);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lTileLocation);
    nwn_ExecuteCommand(514, 3);
}

void SetTileSourceLightColor(const location& lTileLocation, int nSourceLight1Color,
                             int nSourceLight2Color)
{
    nwn_StackPushInteger(nSourceLight2Color);
    nwn_StackPushInteger(nSourceLight1Color);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lTileLocation);
    nwn_ExecuteCommand(515, 3);
}

void RecomputeStaticLighting(object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(516, 1);
}

int GetTileMainLight1Color(const location& lTile)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lTile);
    nwn_ExecuteCommand(517, 1);
    return nwn_StackPopInteger();
}

int GetTileMainLight2Color(const location& lTile)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lTile);
    nwn_ExecuteCommand(518, 1);
    return nwn_StackPopInteger();
}

int GetTileSourceLight1Color(const location& lTile)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lTile);
    nwn_ExecuteCommand(519, 1);
    return nwn_StackPopInteger();
}

int GetTileSourceLight2Color(const location& lTile)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lTile);
    nwn_ExecuteCommand(520, 1);
    return nwn_StackPopInteger();
}

void SetPanelButtonFlash(object oPlayer, int nButton, int nEnableFlash)
{
    nwn_StackPushInteger(nEnableFlash);
    nwn_StackPushInteger(nButton);
    nwn_StackPushObject(oPlayer);
    nwn_ExecuteCommand(521, 3);
}

int GetCurrentAction(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(522, 1);
    return nwn_StackPopInteger();
}

void SetStandardFactionReputation(int nStandardFaction, int nNewReputation, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nNewReputation);
    nwn_StackPushInteger(nStandardFaction);
    nwn_ExecuteCommand(523, 3);
}

int GetStandardFactionReputation(int nStandardFaction, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nStandardFaction);
    nwn_ExecuteCommand(524, 2);
    return nwn_StackPopInteger();
}

void FloatingTextStrRefOnCreature(int nStrRefToDisplay, object oCreatureToFloatAbove,
                                  int bBroadcastToFaction)
{
    nwn_StackPushInteger(bBroadcastToFaction);
    nwn_StackPushObject(oCreatureToFloatAbove);
    nwn_StackPushInteger(nStrRefToDisplay);
    nwn_ExecuteCommand(525, 3);
}

void FloatingTextStringOnCreature(const string& sStringToDisplay, object oCreatureToFloatAbove,
                                  int bBroadcastToFaction)
{
    nwn_StackPushInteger(bBroadcastToFaction);
    nwn_StackPushObject(oCreatureToFloatAbove);
    nwn_StackPushString(sStringToDisplay.c_str());
    nwn_ExecuteCommand(526, 3);
}

int GetTrapDisarmable(object oTrapObject)
{
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(527, 1);
    return nwn_StackPopInteger();
}

int GetTrapDetectable(object oTrapObject)
{
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(528, 1);
    return nwn_StackPopInteger();
}

int GetTrapDetectedBy(object oTrapObject, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(529, 2);
    return nwn_StackPopInteger();
}

int GetTrapFlagged(object oTrapObject)
{
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(530, 1);
    return nwn_StackPopInteger();
}

int GetTrapBaseType(object oTrapObject)
{
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(531, 1);
    return nwn_StackPopInteger();
}

int GetTrapOneShot(object oTrapObject)
{
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(532, 1);
    return nwn_StackPopInteger();
}

object GetTrapCreator(object oTrapObject)
{
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(533, 1);
    return nwn_StackPopObject();
}

string GetTrapKeyTag(object oTrapObject)
{
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(534, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

int GetTrapDisarmDC(object oTrapObject)
{
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(535, 1);
    return nwn_StackPopInteger();
}

int GetTrapDetectDC(object oTrapObject)
{
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(536, 1);
    return nwn_StackPopInteger();
}

int GetLockKeyRequired(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(537, 1);
    return nwn_StackPopInteger();
}

string GetLockKeyTag(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(538, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

int GetLockLockable(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(539, 1);
    return nwn_StackPopInteger();
}

int GetLockUnlockDC(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(540, 1);
    return nwn_StackPopInteger();
}

int GetLockLockDC(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(541, 1);
    return nwn_StackPopInteger();
}

object GetPCLevellingUp()
{
    nwn_ExecuteCommand(542, 0);
    return nwn_StackPopObject();
}

int GetHasFeatEffect(int nFeat, object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_StackPushInteger(nFeat);
    nwn_ExecuteCommand(543, 2);
    return nwn_StackPopInteger();
}

void SetPlaceableIllumination(object oPlaceable, int bIlluminate)
{
    nwn_StackPushInteger(bIlluminate);
    nwn_StackPushObject(oPlaceable);
    nwn_ExecuteCommand(544, 2);
}

int GetPlaceableIllumination(object oPlaceable)
{
    nwn_StackPushObject(oPlaceable);
    nwn_ExecuteCommand(545, 1);
    return nwn_StackPopInteger();
}

int GetIsPlaceableObjectActionPossible(object oPlaceable, int nPlaceableAction)
{
    nwn_StackPushInteger(nPlaceableAction);
    nwn_StackPushObject(oPlaceable);
    nwn_ExecuteCommand(546, 2);
    return nwn_StackPopInteger();
}

void DoPlaceableObjectAction(object oPlaceable, int nPlaceableAction)
{
    nwn_StackPushInteger(nPlaceableAction);
    nwn_StackPushObject(oPlaceable);
    nwn_ExecuteCommand(547, 2);
}

object GetFirstPC()
{
    nwn_ExecuteCommand(548, 0);
    return nwn_StackPopObject();
}

object GetNextPC()
{
    nwn_ExecuteCommand(549, 0);
    return nwn_StackPopObject();
}

int SetTrapDetectedBy(object oTrap, object oDetector, int bDetected)
{
    nwn_StackPushInteger(bDetected);
    nwn_StackPushObject(oDetector);
    nwn_StackPushObject(oTrap);
    nwn_ExecuteCommand(550, 3);
    return nwn_StackPopInteger();
}

int GetIsTrapped(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(551, 1);
    return nwn_StackPopInteger();
}

effect EffectTurnResistanceDecrease(int nHitDice)
{
    nwn_StackPushInteger(nHitDice);
    nwn_ExecuteCommand(552, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

effect EffectTurnResistanceIncrease(int nHitDice)
{
    nwn_StackPushInteger(nHitDice);
    nwn_ExecuteCommand(553, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

void PopUpDeathGUIPanel(object oPC, int bRespawnButtonEnabled, int bWaitForHelpButtonEnabled,
                        int nHelpStringReference, const string& sHelpString)
{
    nwn_StackPushString(sHelpString.c_str());
    nwn_StackPushInteger(nHelpStringReference);
    nwn_StackPushInteger(bWaitForHelpButtonEnabled);
    nwn_StackPushInteger(bRespawnButtonEnabled);
    nwn_StackPushObject(oPC);
    nwn_ExecuteCommand(554, 5);
}

void SetTrapDisabled(object oTrap)
{
    nwn_StackPushObject(oTrap);
    nwn_ExecuteCommand(555, 1);
}

object GetLastHostileActor(object oVictim)
{
    nwn_StackPushObject(oVictim);
    nwn_ExecuteCommand(556, 1);
    return nwn_StackPopObject();
}

void ExportAllCharacters()
{
    nwn_ExecuteCommand(557, 0);
}

int MusicBackgroundGetDayTrack(object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(558, 1);
    return nwn_StackPopInteger();
}

int MusicBackgroundGetNightTrack(object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(559, 1);
    return nwn_StackPopInteger();
}

void WriteTimestampedLogEntry(const string& sLogEntry)
{
    nwn_StackPushString(sLogEntry.c_str());
    nwn_ExecuteCommand(560, 1);
}

string GetModuleName()
{
    nwn_ExecuteCommand(561, 0);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

object GetFactionLeader(object oMemberOfFaction)
{
    nwn_StackPushObject(oMemberOfFaction);
    nwn_ExecuteCommand(562, 1);
    return nwn_StackPopObject();
}

void SendMessageToAllDMs(const string& szMessage)
{
    nwn_StackPushString(szMessage.c_str());
    nwn_ExecuteCommand(563, 1);
}

void EndGame(const string& sEndMovie)
{
    nwn_StackPushString(sEndMovie.c_str());
    nwn_ExecuteCommand(564, 1);
}

void BootPC(object oPlayer)
{
    nwn_StackPushObject(oPlayer);
    nwn_ExecuteCommand(565, 1);
}

void ActionCounterSpell(object oCounterSpellTarget)
{
    nwn_StackPushObject(oCounterSpellTarget);
    nwn_ExecuteCommand(566, 1);
}

void AmbientSoundSetDayVolume(object oArea, int nVolume)
{
    nwn_StackPushInteger(nVolume);
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(567, 2);
}

void AmbientSoundSetNightVolume(object oArea, int nVolume)
{
    nwn_StackPushInteger(nVolume);
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(568, 2);
}

int MusicBackgroundGetBattleTrack(object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(569, 1);
    return nwn_StackPopInteger();
}

int GetHasInventory(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(570, 1);
    return nwn_StackPopInteger();
}

float GetStrRefSoundDuration(int nStrRef)
{
    nwn_StackPushInteger(nStrRef);
    nwn_ExecuteCommand(571, 1);
    return nwn_StackPopFloat();
}

void AddToParty(object oPC, object oPartyLeader)
{
    nwn_StackPushObject(oPartyLeader);
    nwn_StackPushObject(oPC);
    nwn_ExecuteCommand(572, 2);
}

void RemoveFromParty(object oPC)
{
    nwn_StackPushObject(oPC);
    nwn_ExecuteCommand(573, 1);
}

int GetStealthMode(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(574, 1);
    return nwn_StackPopInteger();
}

int GetDetectMode(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(575, 1);
    return nwn_StackPopInteger();
}

int GetDefensiveCastingMode(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(576, 1);
    return nwn_StackPopInteger();
}

int GetAppearanceType(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(577, 1);
    return nwn_StackPopInteger();
}

int GetModuleItemAcquiredStackSize()
{
    nwn_ExecuteCommand(579, 0);
    return nwn_StackPopInteger();
}

void DecrementRemainingFeatUses(object oCreature, int nFeat)
{
    nwn_StackPushInteger(nFeat);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(580, 2);
}

void DecrementRemainingSpellUses(object oCreature, int nSpell)
{
    nwn_StackPushInteger(nSpell);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(581, 2);
}

string GetResRef(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(582, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

effect EffectPetrify()
{
    nwn_ExecuteCommand(583, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

object CopyItem(object oItem, object oTargetInventory, int bCopyVars)
{
    nwn_StackPushInteger(bCopyVars);
    nwn_StackPushObject(oTargetInventory);
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(584, 3);
    return nwn_StackPopObject();
}

effect EffectCutsceneParalyze()
{
    nwn_ExecuteCommand(585, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

int GetDroppableFlag(object oItem)
{
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(586, 1);
    return nwn_StackPopInteger();
}

int GetUseableFlag(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(587, 1);
    return nwn_StackPopInteger();
}

int GetStolenFlag(object oStolen)
{
    nwn_StackPushObject(oStolen);
    nwn_ExecuteCommand(588, 1);
    return nwn_StackPopInteger();
}

void SetCampaignFloat(const string& sCampaignName, const string& sVarName, float flFloat,
                      object oPlayer)
{
    nwn_StackPushObject(oPlayer);
    nwn_StackPushFloat(flFloat);
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushString(sCampaignName.c_str());
    nwn_ExecuteCommand(589, 4);
}

void SetCampaignInt(const string& sCampaignName, const string& sVarName, int nInt, object oPlayer)
{
    nwn_StackPushObject(oPlayer);
    nwn_StackPushInteger(nInt);
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushString(sCampaignName.c_str());
    nwn_ExecuteCommand(590, 4);
}

void SetCampaignvec3(const string& sCampaignName, const string& sVarName, vec3 vvec3,
                     object oPlayer)
{
    nwn_StackPushObject(oPlayer);
    nwn_StackPushVector((Vector*)&vvec3);
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushString(sCampaignName.c_str());
    nwn_ExecuteCommand(591, 4);
}

void SetCampaignLocation(const string& sCampaignName, const string& sVarName,
                         const location& locLocation,
                         object oPlayer)
{
    nwn_StackPushObject(oPlayer);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&locLocation);
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushString(sCampaignName.c_str());
    nwn_ExecuteCommand(592, 4);
}

void SetCampaignString(const string& sCampaignName, const string& sVarName, const string& sString,
                       object oPlayer)
{
    nwn_StackPushObject(oPlayer);
    nwn_StackPushString(sString.c_str());
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushString(sCampaignName.c_str());
    nwn_ExecuteCommand(593, 4);
}

void DestroyCampaignDatabase(const string& sCampaignName)
{
    nwn_StackPushString(sCampaignName.c_str());
    nwn_ExecuteCommand(594, 1);
}

float GetCampaignFloat(const string& sCampaignName, const string& sVarName, object oPlayer)
{
    nwn_StackPushObject(oPlayer);
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushString(sCampaignName.c_str());
    nwn_ExecuteCommand(595, 3);
    return nwn_StackPopFloat();
}

int GetCampaignInt(const string& sCampaignName, const string& sVarName, object oPlayer)
{
    nwn_StackPushObject(oPlayer);
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushString(sCampaignName.c_str());
    nwn_ExecuteCommand(596, 3);
    return nwn_StackPopInteger();
}

vec3 GetCampaignvec3(const string& sCampaignName, const string& sVarName, object oPlayer)
{
    nwn_StackPushObject(oPlayer);
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushString(sCampaignName.c_str());
    nwn_ExecuteCommand(597, 3);
    Vector *v = nwn_StackPopVector();
    vec3 res = { v->X, v->Y, v->Z };
    free(v);
    return res;
}

location GetCampaignLocation(const string& sCampaignName, const string& sVarName, object oPlayer)
{
    nwn_StackPushObject(oPlayer);
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushString(sCampaignName.c_str());
    nwn_ExecuteCommand(598, 3);
    CScriptLocation* l = (CScriptLocation*)nwn_StackPopEngineStructure(ENGINE_STRUCTURE_LOCATION);
    location loc { { l->X, l->Y, l->Z },
        { l->OrientationX, l->OrientationY, l->OrientationZ },
        l->AreaID
    };
    free(l);
    return loc;

}

string GetCampaignString(const string& sCampaignName, const string& sVarName, object oPlayer)
{
    nwn_StackPushObject(oPlayer);
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushString(sCampaignName.c_str());
    nwn_ExecuteCommand(599, 3);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

object CopyObject(object oSource, const location& locLocation, object oOwner,
                  const string& sNewTag)
{
    nwn_StackPushString(sNewTag.c_str());
    nwn_StackPushObject(oOwner);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&locLocation);
    nwn_StackPushObject(oSource);
    nwn_ExecuteCommand(600, 4);
    return nwn_StackPopObject();
}

void DeleteCampaignVariable(const string& sCampaignName, const string& sVarName, object oPlayer)
{
    nwn_StackPushObject(oPlayer);
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushString(sCampaignName.c_str());
    nwn_ExecuteCommand(601, 3);
}

int StoreCampaignObject(const string& sCampaignName, const string& sVarName, object oObject,
                        object oPlayer)
{
    nwn_StackPushObject(oPlayer);
    nwn_StackPushObject(oObject);
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushString(sCampaignName.c_str());
    nwn_ExecuteCommand(602, 4);
    return nwn_StackPopInteger();
}

object RetrieveCampaignObject(const string& sCampaignName, const string& sVarName,
                              const location& locLocation, object oOwner, object oPlayer)
{
    nwn_StackPushObject(oPlayer);
    nwn_StackPushObject(oOwner);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&locLocation);
    nwn_StackPushString(sVarName.c_str());
    nwn_StackPushString(sCampaignName.c_str());
    nwn_ExecuteCommand(603, 5);
    return nwn_StackPopObject();
}

effect EffectCutsceneDominated()
{
    nwn_ExecuteCommand(604, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

int GetItemStackSize(object oItem)
{
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(605, 1);
    return nwn_StackPopInteger();
}

void SetItemStackSize(object oItem, int nSize)
{
    nwn_StackPushInteger(nSize);
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(606, 2);
}

int GetItemCharges(object oItem)
{
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(607, 1);
    return nwn_StackPopInteger();
}

void SetItemCharges(object oItem, int nCharges)
{
    nwn_StackPushInteger(nCharges);
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(608, 2);
}

void AddItemProperty(int nDurationType, const itemproperty& ipProperty, object oItem,
                     float fDuration)
{
    nwn_StackPushFloat(fDuration);
    nwn_StackPushObject(oItem);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY, ipProperty.structure);
    nwn_StackPushInteger(nDurationType);
    nwn_ExecuteCommand(609, 4);
}

void RemoveItemProperty(object oItem, const itemproperty& ipProperty)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY, ipProperty.structure);
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(610, 2);
}

int GetIsItemPropertyValid(const itemproperty& ipProperty)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY, ipProperty.structure);
    nwn_ExecuteCommand(611, 1);
    return nwn_StackPopInteger();
}

itemproperty GetFirstItemProperty(object oItem)
{
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(612, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty GetNextItemProperty(object oItem)
{
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(613, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

int GetItemPropertyType(const itemproperty& ip)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY, ip.structure);
    nwn_ExecuteCommand(614, 1);
    return nwn_StackPopInteger();
}

int GetItemPropertyDurationType(const itemproperty& ip)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY, ip.structure);
    nwn_ExecuteCommand(615, 1);
    return nwn_StackPopInteger();
}

itemproperty ItemPropertyAbilityBonus(int nAbility, int nBonus)
{
    nwn_StackPushInteger(nBonus);
    nwn_StackPushInteger(nAbility);
    nwn_ExecuteCommand(616, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyACBonus(int nBonus)
{
    nwn_StackPushInteger(nBonus);
    nwn_ExecuteCommand(617, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyACBonusVsAlign(int nAlignGroup, int nACBonus)
{
    nwn_StackPushInteger(nACBonus);
    nwn_StackPushInteger(nAlignGroup);
    nwn_ExecuteCommand(618, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyACBonusVsDmgType(int nDamageType, int nACBonus)
{
    nwn_StackPushInteger(nACBonus);
    nwn_StackPushInteger(nDamageType);
    nwn_ExecuteCommand(619, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyACBonusVsRace(int nRace, int nACBonus)
{
    nwn_StackPushInteger(nACBonus);
    nwn_StackPushInteger(nRace);
    nwn_ExecuteCommand(620, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyACBonusVsSAlign(int nAlign, int nACBonus)
{
    nwn_StackPushInteger(nACBonus);
    nwn_StackPushInteger(nAlign);
    nwn_ExecuteCommand(621, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyEnhancementBonus(int nEnhancementBonus)
{
    nwn_StackPushInteger(nEnhancementBonus);
    nwn_ExecuteCommand(622, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyEnhancementBonusVsAlign(int nAlignGroup, int nBonus)
{
    nwn_StackPushInteger(nBonus);
    nwn_StackPushInteger(nAlignGroup);
    nwn_ExecuteCommand(623, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyEnhancementBonusVsRace(int nRace, int nBonus)
{
    nwn_StackPushInteger(nBonus);
    nwn_StackPushInteger(nRace);
    nwn_ExecuteCommand(624, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyEnhancementBonusVsSAlign(int nAlign, int nBonus)
{
    nwn_StackPushInteger(nBonus);
    nwn_StackPushInteger(nAlign);
    nwn_ExecuteCommand(625, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyEnhancementPenalty(int nPenalty)
{
    nwn_StackPushInteger(nPenalty);
    nwn_ExecuteCommand(626, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyWeightReduction(int nReduction)
{
    nwn_StackPushInteger(nReduction);
    nwn_ExecuteCommand(627, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyBonusFeat(int nFeat)
{
    nwn_StackPushInteger(nFeat);
    nwn_ExecuteCommand(628, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyBonusLevelSpell(int nClass, int nSpellLevel)
{
    nwn_StackPushInteger(nSpellLevel);
    nwn_StackPushInteger(nClass);
    nwn_ExecuteCommand(629, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyCastSpell(int nSpell, int nNumUses)
{
    nwn_StackPushInteger(nNumUses);
    nwn_StackPushInteger(nSpell);
    nwn_ExecuteCommand(630, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyDamageBonus(int nDamageType, int nDamage)
{
    nwn_StackPushInteger(nDamage);
    nwn_StackPushInteger(nDamageType);
    nwn_ExecuteCommand(631, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyDamageBonusVsAlign(int nAlignGroup, int nDamageType, int nDamage)
{
    nwn_StackPushInteger(nDamage);
    nwn_StackPushInteger(nDamageType);
    nwn_StackPushInteger(nAlignGroup);
    nwn_ExecuteCommand(632, 3);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyDamageBonusVsRace(int nRace, int nDamageType, int nDamage)
{
    nwn_StackPushInteger(nDamage);
    nwn_StackPushInteger(nDamageType);
    nwn_StackPushInteger(nRace);
    nwn_ExecuteCommand(633, 3);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyDamageBonusVsSAlign(int nAlign, int nDamageType, int nDamage)
{
    nwn_StackPushInteger(nDamage);
    nwn_StackPushInteger(nDamageType);
    nwn_StackPushInteger(nAlign);
    nwn_ExecuteCommand(634, 3);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyDamageImmunity(int nDamageType, int nImmuneBonus)
{
    nwn_StackPushInteger(nImmuneBonus);
    nwn_StackPushInteger(nDamageType);
    nwn_ExecuteCommand(635, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyDamagePenalty(int nPenalty)
{
    nwn_StackPushInteger(nPenalty);
    nwn_ExecuteCommand(636, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyDamageReduction(int nEnhancement, int nHPSoak)
{
    nwn_StackPushInteger(nHPSoak);
    nwn_StackPushInteger(nEnhancement);
    nwn_ExecuteCommand(637, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyDamageResistance(int nDamageType, int nHPResist)
{
    nwn_StackPushInteger(nHPResist);
    nwn_StackPushInteger(nDamageType);
    nwn_ExecuteCommand(638, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyDamageVulnerability(int nDamageType, int nVulnerability)
{
    nwn_StackPushInteger(nVulnerability);
    nwn_StackPushInteger(nDamageType);
    nwn_ExecuteCommand(639, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyDarkvision()
{
    nwn_ExecuteCommand(640, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyDecreaseAbility(int nAbility, int nModifier)
{
    nwn_StackPushInteger(nModifier);
    nwn_StackPushInteger(nAbility);
    nwn_ExecuteCommand(641, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyDecreaseAC(int nModifierType, int nPenalty)
{
    nwn_StackPushInteger(nPenalty);
    nwn_StackPushInteger(nModifierType);
    nwn_ExecuteCommand(642, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyDecreaseSkill(int nSkill, int nPenalty)
{
    nwn_StackPushInteger(nPenalty);
    nwn_StackPushInteger(nSkill);
    nwn_ExecuteCommand(643, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyContainerReducedWeight(int nContainerType)
{
    nwn_StackPushInteger(nContainerType);
    nwn_ExecuteCommand(644, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyExtraMeleeDamageType(int nDamageType)
{
    nwn_StackPushInteger(nDamageType);
    nwn_ExecuteCommand(645, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyExtraRangeDamageType(int nDamageType)
{
    nwn_StackPushInteger(nDamageType);
    nwn_ExecuteCommand(646, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyHaste()
{
    nwn_ExecuteCommand(647, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyHolyAvenger()
{
    nwn_ExecuteCommand(648, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyImmunityMisc(int nImmunityType)
{
    nwn_StackPushInteger(nImmunityType);
    nwn_ExecuteCommand(649, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyImprovedEvasion()
{
    nwn_ExecuteCommand(650, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyBonusSpellResistance(int nBonus)
{
    nwn_StackPushInteger(nBonus);
    nwn_ExecuteCommand(651, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyBonusSavingThrowVsX(int nBonusType, int nBonus)
{
    nwn_StackPushInteger(nBonus);
    nwn_StackPushInteger(nBonusType);
    nwn_ExecuteCommand(652, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyBonusSavingThrow(int nBaseSaveType, int nBonus)
{
    nwn_StackPushInteger(nBonus);
    nwn_StackPushInteger(nBaseSaveType);
    nwn_ExecuteCommand(653, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyKeen()
{
    nwn_ExecuteCommand(654, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyLight(int nBrightness, int nColor)
{
    nwn_StackPushInteger(nColor);
    nwn_StackPushInteger(nBrightness);
    nwn_ExecuteCommand(655, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyMaxRangeStrengthMod(int nModifier)
{
    nwn_StackPushInteger(nModifier);
    nwn_ExecuteCommand(656, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyNoDamage()
{
    nwn_ExecuteCommand(657, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyOnHitProps(int nProperty, int nSaveDC, int nSpecial)
{
    nwn_StackPushInteger(nSpecial);
    nwn_StackPushInteger(nSaveDC);
    nwn_StackPushInteger(nProperty);
    nwn_ExecuteCommand(658, 3);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyReducedSavingThrowVsX(int nBaseSaveType, int nPenalty)
{
    nwn_StackPushInteger(nPenalty);
    nwn_StackPushInteger(nBaseSaveType);
    nwn_ExecuteCommand(659, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyReducedSavingThrow(int nBonusType, int nPenalty)
{
    nwn_StackPushInteger(nPenalty);
    nwn_StackPushInteger(nBonusType);
    nwn_ExecuteCommand(660, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyRegeneration(int nRegenAmount)
{
    nwn_StackPushInteger(nRegenAmount);
    nwn_ExecuteCommand(661, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertySkillBonus(int nSkill, int nBonus)
{
    nwn_StackPushInteger(nBonus);
    nwn_StackPushInteger(nSkill);
    nwn_ExecuteCommand(662, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertySpellImmunitySpecific(int nSpell)
{
    nwn_StackPushInteger(nSpell);
    nwn_ExecuteCommand(663, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertySpellImmunitySchool(int nSchool)
{
    nwn_StackPushInteger(nSchool);
    nwn_ExecuteCommand(664, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyThievesTools(int nModifier)
{
    nwn_StackPushInteger(nModifier);
    nwn_ExecuteCommand(665, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyAttackBonus(int nBonus)
{
    nwn_StackPushInteger(nBonus);
    nwn_ExecuteCommand(666, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyAttackBonusVsAlign(int nAlignGroup, int nBonus)
{
    nwn_StackPushInteger(nBonus);
    nwn_StackPushInteger(nAlignGroup);
    nwn_ExecuteCommand(667, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyAttackBonusVsRace(int nRace, int nBonus)
{
    nwn_StackPushInteger(nBonus);
    nwn_StackPushInteger(nRace);
    nwn_ExecuteCommand(668, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyAttackBonusVsSAlign(int nAlignment, int nBonus)
{
    nwn_StackPushInteger(nBonus);
    nwn_StackPushInteger(nAlignment);
    nwn_ExecuteCommand(669, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyAttackPenalty(int nPenalty)
{
    nwn_StackPushInteger(nPenalty);
    nwn_ExecuteCommand(670, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyUnlimitedAmmo(int nAmmoDamage)
{
    nwn_StackPushInteger(nAmmoDamage);
    nwn_ExecuteCommand(671, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyLimitUseByAlign(int nAlignGroup)
{
    nwn_StackPushInteger(nAlignGroup);
    nwn_ExecuteCommand(672, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyLimitUseByClass(int nClass)
{
    nwn_StackPushInteger(nClass);
    nwn_ExecuteCommand(673, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyLimitUseByRace(int nRace)
{
    nwn_StackPushInteger(nRace);
    nwn_ExecuteCommand(674, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyLimitUseBySAlign(int nAlignment)
{
    nwn_StackPushInteger(nAlignment);
    nwn_ExecuteCommand(675, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyVampiricRegeneration(int nRegenAmount)
{
    nwn_StackPushInteger(nRegenAmount);
    nwn_ExecuteCommand(677, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyTrap(int nTrapLevel, int nTrapType)
{
    nwn_StackPushInteger(nTrapType);
    nwn_StackPushInteger(nTrapLevel);
    nwn_ExecuteCommand(678, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyTrueSeeing()
{
    nwn_ExecuteCommand(679, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyOnMonsterHitProperties(int nProperty, int nSpecial)
{
    nwn_StackPushInteger(nSpecial);
    nwn_StackPushInteger(nProperty);
    nwn_ExecuteCommand(680, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyTurnResistance(int nModifier)
{
    nwn_StackPushInteger(nModifier);
    nwn_ExecuteCommand(681, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyMassiveCritical(int nDamage)
{
    nwn_StackPushInteger(nDamage);
    nwn_ExecuteCommand(682, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyFreeAction()
{
    nwn_ExecuteCommand(683, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyMonsterDamage(int nDamage)
{
    nwn_StackPushInteger(nDamage);
    nwn_ExecuteCommand(684, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyImmunityToSpellLevel(int nLevel)
{
    nwn_StackPushInteger(nLevel);
    nwn_ExecuteCommand(685, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertySpecialWalk(int nWalkType)
{
    nwn_StackPushInteger(nWalkType);
    nwn_ExecuteCommand(686, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyHealersKit(int nModifier)
{
    nwn_StackPushInteger(nModifier);
    nwn_ExecuteCommand(687, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyWeightIncrease(int nWeight)
{
    nwn_StackPushInteger(nWeight);
    nwn_ExecuteCommand(688, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

int GetIsSkillSuccessful(object oTarget, int nSkill, int nDifficulty)
{
    nwn_StackPushInteger(nDifficulty);
    nwn_StackPushInteger(nSkill);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(689, 3);
    return nwn_StackPopInteger();
}

effect EffectSpellFailure(int nPercent, int nSpellSchool)
{
    nwn_StackPushInteger(nSpellSchool);
    nwn_StackPushInteger(nPercent);
    nwn_ExecuteCommand(690, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

void SpeakStringByStrRef(int nStrRef, int nTalkVolume)
{
    nwn_StackPushInteger(nTalkVolume);
    nwn_StackPushInteger(nStrRef);
    nwn_ExecuteCommand(691, 2);
}

void SetCutsceneMode(object oCreature, int nInCutscene, int nLeftClickingEnabled)
{
    nwn_StackPushInteger(nLeftClickingEnabled);
    nwn_StackPushInteger(nInCutscene);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(692, 3);
}

object GetLastPCToCancelCutscene()
{
    nwn_ExecuteCommand(693, 0);
    return nwn_StackPopObject();
}

float GetDialogSoundLength(int nStrRef)
{
    nwn_StackPushInteger(nStrRef);
    nwn_ExecuteCommand(694, 1);
    return nwn_StackPopFloat();
}

void FadeFromBlack(object oCreature, float fSpeed)
{
    nwn_StackPushFloat(fSpeed);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(695, 2);
}

void FadeToBlack(object oCreature, float fSpeed)
{
    nwn_StackPushFloat(fSpeed);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(696, 2);
}

void StopFade(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(697, 1);
}

void BlackScreen(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(698, 1);
}

int GetBaseAttackBonus(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(699, 1);
    return nwn_StackPopInteger();
}

void SetImmortal(object oCreature, int bImmortal)
{
    nwn_StackPushInteger(bImmortal);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(700, 2);
}

void OpenInventory(object oCreature, object oPlayer)
{
    nwn_StackPushObject(oPlayer);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(701, 2);
}

void StoreCameraFacing()
{
    nwn_ExecuteCommand(702, 0);
}

void RestoreCameraFacing()
{
    nwn_ExecuteCommand(703, 0);
}

int LevelUpHenchman(object oCreature, int nClass, int bReadyAllSpells, int nPackage)
{
    nwn_StackPushInteger(nPackage);
    nwn_StackPushInteger(bReadyAllSpells);
    nwn_StackPushInteger(nClass);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(704, 4);
    return nwn_StackPopInteger();
}

void SetDroppableFlag(object oItem, int bDroppable)
{
    nwn_StackPushInteger(bDroppable);
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(705, 2);
}

int GetWeight(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(706, 1);
    return nwn_StackPopInteger();
}

object GetModuleItemAcquiredBy()
{
    nwn_ExecuteCommand(707, 0);
    return nwn_StackPopObject();
}

int GetImmortal(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(708, 1);
    return nwn_StackPopInteger();
}

void DoWhirlwindAttack(int bDisplayFeedback, int bImproved)
{
    nwn_StackPushInteger(bImproved);
    nwn_StackPushInteger(bDisplayFeedback);
    nwn_ExecuteCommand(709, 2);
}

string Get2DAString(const string& s2DA, const string& sColumn, int nRow)
{
    nwn_StackPushInteger(nRow);
    nwn_StackPushString(sColumn.c_str());
    nwn_StackPushString(s2DA.c_str());
    nwn_ExecuteCommand(710, 3);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

effect EffectEthereal()
{
    nwn_ExecuteCommand(711, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

int GetAILevel(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(712, 1);
    return nwn_StackPopInteger();
}

void SetAILevel(object oTarget, int nAILevel)
{
    nwn_StackPushInteger(nAILevel);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(713, 2);
}

int GetIsPossessedFamiliar(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(714, 1);
    return nwn_StackPopInteger();
}

void UnpossessFamiliar(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(715, 1);
}

int GetIsAreaInterior(object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(716, 1);
    return nwn_StackPopInteger();
}

void SendMessageToPCByStrRef(object oPlayer, int nStrRef)
{
    nwn_StackPushInteger(nStrRef);
    nwn_StackPushObject(oPlayer);
    nwn_ExecuteCommand(717, 2);
}

void IncrementRemainingFeatUses(object oCreature, int nFeat)
{
    nwn_StackPushInteger(nFeat);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(718, 2);
}

void ExportSingleCharacter(object oPlayer)
{
    nwn_StackPushObject(oPlayer);
    nwn_ExecuteCommand(719, 1);
}

void PlaySoundByStrRef(int nStrRef, int nRunAsAction)
{
    nwn_StackPushInteger(nRunAsAction);
    nwn_StackPushInteger(nStrRef);
    nwn_ExecuteCommand(720, 2);
}

void SetSubRace(object oCreature, const string& sSubRace)
{
    nwn_StackPushString(sSubRace.c_str());
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(721, 2);
}

void SetDeity(object oCreature, const string& sDeity)
{
    nwn_StackPushString(sDeity.c_str());
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(722, 2);
}

int GetIsDMPossessed(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(723, 1);
    return nwn_StackPopInteger();
}

int GetWeather(object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(724, 1);
    return nwn_StackPopInteger();
}

int GetIsAreaNatural(object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(725, 1);
    return nwn_StackPopInteger();
}

int GetIsAreaAboveGround(object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(726, 1);
    return nwn_StackPopInteger();
}

object GetPCItemLastEquipped()
{
    nwn_ExecuteCommand(727, 0);
    return nwn_StackPopObject();
}

object GetPCItemLastEquippedBy()
{
    nwn_ExecuteCommand(728, 0);
    return nwn_StackPopObject();
}

object GetPCItemLastUnequipped()
{
    nwn_ExecuteCommand(729, 0);
    return nwn_StackPopObject();
}

object GetPCItemLastUnequippedBy()
{
    nwn_ExecuteCommand(730, 0);
    return nwn_StackPopObject();
}

object CopyItemAndModify(object oItem, int nType, int nIndex, int nNewValue, int bCopyVars)
{
    nwn_StackPushInteger(bCopyVars);
    nwn_StackPushInteger(nNewValue);
    nwn_StackPushInteger(nIndex);
    nwn_StackPushInteger(nType);
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(731, 5);
    return nwn_StackPopObject();
}

int GetItemAppearance(object oItem, int nType, int nIndex)
{
    nwn_StackPushInteger(nIndex);
    nwn_StackPushInteger(nType);
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(732, 3);
    return nwn_StackPopInteger();
}

itemproperty ItemPropertyOnHitCastSpell(int nSpell, int nLevel)
{
    nwn_StackPushInteger(nLevel);
    nwn_StackPushInteger(nSpell);
    nwn_ExecuteCommand(733, 2);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

int GetItemPropertySubType(const itemproperty& iProperty)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY, iProperty.structure);
    nwn_ExecuteCommand(734, 1);
    return nwn_StackPopInteger();
}

int GetActionMode(object oCreature, int nMode)
{
    nwn_StackPushInteger(nMode);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(735, 2);
    return nwn_StackPopInteger();
}

void SetActionMode(object oCreature, int nMode, int nStatus)
{
    nwn_StackPushInteger(nStatus);
    nwn_StackPushInteger(nMode);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(736, 3);
}

int GetArcaneSpellFailure(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(737, 1);
    return nwn_StackPopInteger();
}

void ActionExamine(object oExamine)
{
    nwn_StackPushObject(oExamine);
    nwn_ExecuteCommand(738, 1);
}

itemproperty ItemPropertyVisualEffect(int nEffect)
{
    nwn_StackPushInteger(nEffect);
    nwn_ExecuteCommand(739, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

void SetLootable(object oCreature, int bLootable)
{
    nwn_StackPushInteger(bLootable);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(740, 2);
}

int GetLootable(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(741, 1);
    return nwn_StackPopInteger();
}

float GetCutsceneCameraMoveRate(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(742, 1);
    return nwn_StackPopFloat();
}

void SetCutsceneCameraMoveRate(object oCreature, float fRate)
{
    nwn_StackPushFloat(fRate);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(743, 2);
}

int GetItemCursedFlag(object oItem)
{
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(744, 1);
    return nwn_StackPopInteger();
}

void SetItemCursedFlag(object oItem, int nCursed)
{
    nwn_StackPushInteger(nCursed);
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(745, 2);
}

void SetMaxHenchmen(int nNumHenchmen)
{
    nwn_StackPushInteger(nNumHenchmen);
    nwn_ExecuteCommand(746, 1);
}

int GetMaxHenchmen()
{
    nwn_ExecuteCommand(747, 0);
    return nwn_StackPopInteger();
}

int GetAssociateType(object oAssociate)
{
    nwn_StackPushObject(oAssociate);
    nwn_ExecuteCommand(748, 1);
    return nwn_StackPopInteger();
}

int GetSpellResistance(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(749, 1);
    return nwn_StackPopInteger();
}

void DayToNight(object oPlayer, float fTransitionTime)
{
    nwn_StackPushFloat(fTransitionTime);
    nwn_StackPushObject(oPlayer);
    nwn_ExecuteCommand(750, 2);
}

void NightToDay(object oPlayer, float fTransitionTime)
{
    nwn_StackPushFloat(fTransitionTime);
    nwn_StackPushObject(oPlayer);
    nwn_ExecuteCommand(751, 2);
}

int LineOfSightObject(object oSource, object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_StackPushObject(oSource);
    nwn_ExecuteCommand(752, 2);
    return nwn_StackPopInteger();
}

int LineOfSightvec3(vec3 vSource, vec3 vTarget)
{
    nwn_StackPushVector((Vector*)&vTarget);
    nwn_StackPushVector((Vector*)&vSource);
    nwn_ExecuteCommand(753, 2);
    return nwn_StackPopInteger();
}

int GetLastSpellCastClass()
{
    nwn_ExecuteCommand(754, 0);
    return nwn_StackPopInteger();
}

void SetBaseAttackBonus(int nBaseAttackBonus, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nBaseAttackBonus);
    nwn_ExecuteCommand(755, 2);
}

void RestoreBaseAttackBonus(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(756, 1);
}

effect EffectCutsceneGhost()
{
    nwn_ExecuteCommand(757, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

itemproperty ItemPropertyArcaneSpellFailure(int nModLevel)
{
    nwn_StackPushInteger(nModLevel);
    nwn_ExecuteCommand(758, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

int GetStoreGold(object oidStore)
{
    nwn_StackPushObject(oidStore);
    nwn_ExecuteCommand(759, 1);
    return nwn_StackPopInteger();
}

void SetStoreGold(object oidStore, int nGold)
{
    nwn_StackPushInteger(nGold);
    nwn_StackPushObject(oidStore);
    nwn_ExecuteCommand(760, 2);
}

int GetStoreMaxBuyPrice(object oidStore)
{
    nwn_StackPushObject(oidStore);
    nwn_ExecuteCommand(761, 1);
    return nwn_StackPopInteger();
}

void SetStoreMaxBuyPrice(object oidStore, int nMaxBuy)
{
    nwn_StackPushInteger(nMaxBuy);
    nwn_StackPushObject(oidStore);
    nwn_ExecuteCommand(762, 2);
}

int GetStoreIdentifyCost(object oidStore)
{
    nwn_StackPushObject(oidStore);
    nwn_ExecuteCommand(763, 1);
    return nwn_StackPopInteger();
}

void SetStoreIdentifyCost(object oidStore, int nCost)
{
    nwn_StackPushInteger(nCost);
    nwn_StackPushObject(oidStore);
    nwn_ExecuteCommand(764, 2);
}

void SetCreatureAppearanceType(object oCreature, int nAppearanceType)
{
    nwn_StackPushInteger(nAppearanceType);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(765, 2);
}

int GetCreatureStartingPackage(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(766, 1);
    return nwn_StackPopInteger();
}

effect EffectCutsceneImmobilize()
{
    nwn_ExecuteCommand(767, 0);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT);
}

int GetIsInSubArea(object oCreature, object oSubArea)
{
    nwn_StackPushObject(oSubArea);
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(768, 2);
    return nwn_StackPopInteger();
}

int GetItemPropertyCostTable(const itemproperty& iProp)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY, iProp.structure);
    nwn_ExecuteCommand(769, 1);
    return nwn_StackPopInteger();
}

int GetItemPropertyCostTableValue(const itemproperty& iProp)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY, iProp.structure);
    nwn_ExecuteCommand(770, 1);
    return nwn_StackPopInteger();
}

int GetItemPropertyParam1(const itemproperty& iProp)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY, iProp.structure);
    nwn_ExecuteCommand(771, 1);
    return nwn_StackPopInteger();
}

int GetItemPropertyParam1Value(const itemproperty& iProp)
{
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY, iProp.structure);
    nwn_ExecuteCommand(772, 1);
    return nwn_StackPopInteger();
}

int GetIsCreatureDisarmable(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(773, 1);
    return nwn_StackPopInteger();
}

void SetStolenFlag(object oItem, int nStolenFlag)
{
    nwn_StackPushInteger(nStolenFlag);
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(774, 2);
}

void ForceRest(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(775, 1);
}

void SetCameraHeight(object oPlayer, float fHeight)
{
    nwn_StackPushFloat(fHeight);
    nwn_StackPushObject(oPlayer);
    nwn_ExecuteCommand(776, 2);
}

void SetSkyBox(int nSkyBox, object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_StackPushInteger(nSkyBox);
    nwn_ExecuteCommand(777, 2);
}

int GetPhenoType(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(778, 1);
    return nwn_StackPopInteger();
}

void SetPhenoType(int nPhenoType, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nPhenoType);
    nwn_ExecuteCommand(779, 2);
}

void SetFogColor(int nFogType, int nFogColor, object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_StackPushInteger(nFogColor);
    nwn_StackPushInteger(nFogType);
    nwn_ExecuteCommand(780, 3);
}

int GetCutsceneMode(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(781, 1);
    return nwn_StackPopInteger();
}

int GetSkyBox(object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(782, 1);
    return nwn_StackPopInteger();
}

int GetFogColor(int nFogType, object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_StackPushInteger(nFogType);
    nwn_ExecuteCommand(783, 2);
    return nwn_StackPopInteger();
}

void SetFogAmount(int nFogType, int nFogAmount, object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_StackPushInteger(nFogAmount);
    nwn_StackPushInteger(nFogType);
    nwn_ExecuteCommand(784, 3);
}

int GetFogAmount(int nFogType, object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_StackPushInteger(nFogType);
    nwn_ExecuteCommand(785, 2);
    return nwn_StackPopInteger();
}

int GetPickpocketableFlag(object oItem)
{
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(786, 1);
    return nwn_StackPopInteger();
}

void SetPickpocketableFlag(object oItem, int bPickpocketable)
{
    nwn_StackPushInteger(bPickpocketable);
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(787, 2);
}

int GetFootstepType(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(788, 1);
    return nwn_StackPopInteger();
}

void SetFootstepType(int nFootstepType, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nFootstepType);
    nwn_ExecuteCommand(789, 2);
}

int GetCreatureWingType(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(790, 1);
    return nwn_StackPopInteger();
}

void SetCreatureWingType(int nWingType, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nWingType);
    nwn_ExecuteCommand(791, 2);
}

int GetCreatureBodyPart(int nPart, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nPart);
    nwn_ExecuteCommand(792, 2);
    return nwn_StackPopInteger();
}

void SetCreatureBodyPart(int nPart, int nModelNumber, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nModelNumber);
    nwn_StackPushInteger(nPart);
    nwn_ExecuteCommand(793, 3);
}

int GetCreatureTailType(object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_ExecuteCommand(794, 1);
    return nwn_StackPopInteger();
}

void SetCreatureTailType(int nTailType, object oCreature)
{
    nwn_StackPushObject(oCreature);
    nwn_StackPushInteger(nTailType);
    nwn_ExecuteCommand(795, 2);
}

int GetHardness(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(796, 1);
    return nwn_StackPopInteger();
}

void SetHardness(int nHardness, object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_StackPushInteger(nHardness);
    nwn_ExecuteCommand(797, 2);
}

void SetLockKeyRequired(object oObject, int nKeyRequired)
{
    nwn_StackPushInteger(nKeyRequired);
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(798, 2);
}

void SetLockKeyTag(object oObject, const string& sNewKeyTag)
{
    nwn_StackPushString(sNewKeyTag.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(799, 2);
}

void SetLockLockable(object oObject, int nLockable)
{
    nwn_StackPushInteger(nLockable);
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(800, 2);
}

void SetLockUnlockDC(object oObject, int nNewUnlockDC)
{
    nwn_StackPushInteger(nNewUnlockDC);
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(801, 2);
}

void SetLockLockDC(object oObject, int nNewLockDC)
{
    nwn_StackPushInteger(nNewLockDC);
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(802, 2);
}

void SetTrapDisarmable(object oTrapObject, int nDisarmable)
{
    nwn_StackPushInteger(nDisarmable);
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(803, 2);
}

void SetTrapDetectable(object oTrapObject, int nDetectable)
{
    nwn_StackPushInteger(nDetectable);
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(804, 2);
}

void SetTrapOneShot(object oTrapObject, int nOneShot)
{
    nwn_StackPushInteger(nOneShot);
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(805, 2);
}

void SetTrapKeyTag(object oTrapObject, const string& sKeyTag)
{
    nwn_StackPushString(sKeyTag.c_str());
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(806, 2);
}

void SetTrapDisarmDC(object oTrapObject, int nDisarmDC)
{
    nwn_StackPushInteger(nDisarmDC);
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(807, 2);
}

void SetTrapDetectDC(object oTrapObject, int nDetectDC)
{
    nwn_StackPushInteger(nDetectDC);
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(808, 2);
}

object CreateTrapAtLocation(int nTrapType, const location& lLocation, float fSize,
                            const string& sTag,
                            int nFaction, const string& sOnDisarmScript,
                            const string& sOnTrapTriggeredScript)
{
    nwn_StackPushString(sOnTrapTriggeredScript.c_str());
    nwn_StackPushString(sOnDisarmScript.c_str());
    nwn_StackPushInteger(nFaction);
    nwn_StackPushString(sTag.c_str());
    nwn_StackPushFloat(fSize);
    nwn_StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, (CScriptLocation*)&lLocation);
    nwn_StackPushInteger(nTrapType);
    nwn_ExecuteCommand(809, 7);
    return nwn_StackPopObject();
}

void CreateTrapOnObject(int nTrapType, object oObject, int nFaction, const string& sOnDisarmScript,
                        const string& sOnTrapTriggeredScript)
{
    nwn_StackPushString(sOnTrapTriggeredScript.c_str());
    nwn_StackPushString(sOnDisarmScript.c_str());
    nwn_StackPushInteger(nFaction);
    nwn_StackPushObject(oObject);
    nwn_StackPushInteger(nTrapType);
    nwn_ExecuteCommand(810, 5);
}

void SetWillSavingThrow(object oObject, int nWillSave)
{
    nwn_StackPushInteger(nWillSave);
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(811, 2);
}

void SetReflexSavingThrow(object oObject, int nReflexSave)
{
    nwn_StackPushInteger(nReflexSave);
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(812, 2);
}

void SetFortitudeSavingThrow(object oObject, int nFortitudeSave)
{
    nwn_StackPushInteger(nFortitudeSave);
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(813, 2);
}

string GetTilesetResRef(object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_ExecuteCommand(814, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

int GetTrapRecoverable(object oTrapObject)
{
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(815, 1);
    return nwn_StackPopInteger();
}

void SetTrapRecoverable(object oTrapObject, int nRecoverable)
{
    nwn_StackPushInteger(nRecoverable);
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(816, 2);
}

int GetModuleXPScale()
{
    nwn_ExecuteCommand(817, 0);
    return nwn_StackPopInteger();
}

void SetModuleXPScale(int nXPScale)
{
    nwn_StackPushInteger(nXPScale);
    nwn_ExecuteCommand(818, 1);
}

string GetKeyRequiredFeedback(object oObject)
{
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(819, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

void SetKeyRequiredFeedback(object oObject, const string& sFeedbackMessage)
{
    nwn_StackPushString(sFeedbackMessage.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(820, 2);
}

int GetTrapActive(object oTrapObject)
{
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(821, 1);
    return nwn_StackPopInteger();
}

void SetTrapActive(object oTrapObject, int nActive)
{
    nwn_StackPushInteger(nActive);
    nwn_StackPushObject(oTrapObject);
    nwn_ExecuteCommand(822, 2);
}

void LockCameraPitch(object oPlayer, int bLocked)
{
    nwn_StackPushInteger(bLocked);
    nwn_StackPushObject(oPlayer);
    nwn_ExecuteCommand(823, 2);
}

void LockCameraDistance(object oPlayer, int bLocked)
{
    nwn_StackPushInteger(bLocked);
    nwn_StackPushObject(oPlayer);
    nwn_ExecuteCommand(824, 2);
}

void LockCameraDirection(object oPlayer, int bLocked)
{
    nwn_StackPushInteger(bLocked);
    nwn_StackPushObject(oPlayer);
    nwn_ExecuteCommand(825, 2);
}

object GetPlaceableLastClickedBy()
{
    nwn_ExecuteCommand(826, 0);
    return nwn_StackPopObject();
}

int GetInfiniteFlag(object oItem)
{
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(827, 1);
    return nwn_StackPopInteger();
}

void SetInfiniteFlag(object oItem, int bInfinite)
{
    nwn_StackPushInteger(bInfinite);
    nwn_StackPushObject(oItem);
    nwn_ExecuteCommand(828, 2);
}

int GetAreaSize(int nAreaDimension, object oArea)
{
    nwn_StackPushObject(oArea);
    nwn_StackPushInteger(nAreaDimension);
    nwn_ExecuteCommand(829, 2);
    return nwn_StackPopInteger();
}

void SetName(object oObject, const string& sNewName)
{
    nwn_StackPushString(sNewName.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(830, 2);
}

int GetPortraitId(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(831, 1);
    return nwn_StackPopInteger();
}

void SetPortraitId(object oTarget, int nPortraitId)
{
    nwn_StackPushInteger(nPortraitId);
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(832, 2);
}

string GetPortraitResRef(object oTarget)
{
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(833, 1);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

void SetPortraitResRef(object oTarget, const string& sPortraitResRef)
{
    nwn_StackPushString(sPortraitResRef.c_str());
    nwn_StackPushObject(oTarget);
    nwn_ExecuteCommand(834, 2);
}

void SetUseableFlag(object oPlaceable, int nUseableFlag)
{
    nwn_StackPushInteger(nUseableFlag);
    nwn_StackPushObject(oPlaceable);
    nwn_ExecuteCommand(835, 2);
}

string GetDescription(object oObject, int bOriginalDescription, int bIdentifiedDescription)
{
    nwn_StackPushInteger(bIdentifiedDescription);
    nwn_StackPushInteger(bOriginalDescription);
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(836, 3);
    char *str = nwn_StackPopString();
    string res(str);
    free(str);
    return res;
}

void SetDescription(object oObject, const string& sNewDescription, int bIdentifiedDescription)
{
    nwn_StackPushInteger(bIdentifiedDescription);
    nwn_StackPushString(sNewDescription.c_str());
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(837, 3);
}

object GetPCChatSpeaker()
{
    nwn_ExecuteCommand(838, 0);
    return nwn_StackPopObject();
}

string GetPCChatMessage()
{
    nwn_ExecuteCommand(839, 0);
    return nwn_StackPopString();
}

int GetPCChatVolume()
{
    nwn_ExecuteCommand(840, 0);
    return nwn_StackPopInteger();
}

void SetPCChatMessage(const string& sNewChatMessage)
{
    nwn_StackPushString(sNewChatMessage.c_str());
    nwn_ExecuteCommand(841, 1);
}

void SetPCChatVolume(int nTalkVolume)
{
    nwn_StackPushInteger(nTalkVolume);
    nwn_ExecuteCommand(842, 1);
}

int GetColor(object oObject, int nColorChannel)
{
    nwn_StackPushInteger(nColorChannel);
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(843, 2);
    return nwn_StackPopInteger();
}

void SetColor(object oObject, int nColorChannel, int nColorValue)
{
    nwn_StackPushInteger(nColorValue);
    nwn_StackPushInteger(nColorChannel);
    nwn_StackPushObject(oObject);
    nwn_ExecuteCommand(844, 3);
}

itemproperty ItemPropertyMaterial(int nMaterialType)
{
    nwn_StackPushInteger(nMaterialType);
    nwn_ExecuteCommand(845, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyQuality(int nQuality)
{
    nwn_StackPushInteger(nQuality);
    nwn_ExecuteCommand(846, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}

itemproperty ItemPropertyAdditional(int nAdditionalProperty)
{
    nwn_StackPushInteger(nAdditionalProperty);
    nwn_ExecuteCommand(847, 1);
    return nwn_StackPopEngineStructure(ENGINE_STRUCTURE_ITEMPROPERTY);
}
