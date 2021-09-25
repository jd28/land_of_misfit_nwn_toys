#include "NWNXCombat.h"

extern CNWNXCombat combat;

int32_t Hook_ExecuteCommandDoTouchAttack(CNWVirtualMachineCommands *vmc, int32_t cmd, int32_t args) {
    bool is_ranged = cmd == 147;
    uint32_t attacker_id = vmc->vmc_object_id;

    uint32_t target_id;
    int32_t feedback = 1;
        
    if ( !CVirtualMachine__StackPopObject(*NWN_VirtualMachine, &target_id) ||
         ( args > 1 && !CVirtualMachine__StackPopInteger(*NWN_VirtualMachine, &feedback) ) ) {
        return -639;
    }

    auto att = combat.get_creature(attacker_id);
    if ( !att ) { return 0; }
    
    CGameObject *target_ob  = nwn_GetObjectByID(target_id);
    if ( target_ob == NULL ) { return 0; }

    CNWSObject  *target_obj = target_ob->vtable->AsNWSObject(target_ob);
    if ( target_obj == NULL ) { return 0; }
    
    SituatedObject obj(target_obj);
    Defense *def;
    bool target_is_cre = target_ob->type == OBJECT_TYPE_CREATURE;

    if ( target_is_cre ) {
        auto tar = combat.get_creature(target_id);
        def = &tar->defense;
    }
    else {
        def = &obj;
    }

    uint32_t state = GetTargetState(nwn_GetCreatureByID(attacker_id),
                                    target_obj);
    bool    dexed = def->canUseDexAC(state, is_ranged);
    int32_t ab    = att->offense.getTouchAttackBonus(is_ranged);
    int32_t ac    = def->getArmorClass(dexed, 0, true);
    int32_t roll  = true_random(1, 20);
    int32_t attack_result = 0;
    int32_t conceal = 0;
    int32_t result = 0;

    if ( roll + ab >= ac ) {
        attack_result = result = 1;
        // Critcal... All touch attacks have a crit threat of 1.
        if ( roll == 20 && true_random(1, 20) + ab >= ac ) {
            result = 2;
        }
    }

    // Resolve defense stuff...

    if ( feedback ) {
        CNWCCMessageData *msg = CNWCCMessageData_create();
        CExoArrayList_int32_add(&msg->integers, ab); // 0
        CExoArrayList_int32_add(&msg->integers, roll); // 1
        CExoArrayList_int32_add(&msg->integers, attack_result); // 2

        if ( is_ranged ) {
            CExoArrayList_int32_add(&msg->integers, 1); // 3
        }

        if ( conceal ) { 
            CExoArrayList_int32_add(&msg->integers, conceal);
        }

        CExoArrayList_uint32_add(&msg->objects, attacker_id);
        CExoArrayList_uint32_add(&msg->objects, target_id);
        
        CNWSPlayer *client_att = CServerExoApp__GetClientObjectByObjectId((*NWN_AppManager)->app_server,
                                                                          attacker_id);
        if ( client_att ) {
            CNWSMessage* message = CServerExoApp__GetNWSMessage((*NWN_AppManager)->app_server);
            CNWSMessage__SendServerToPlayerCCMessage(message, client_att->pl_id, 13, msg, 0);
        }

        CNWSPlayer *client_tar = CServerExoApp__GetClientObjectByObjectId((*NWN_AppManager)->app_server,
                                                                          target_id);
        if ( client_tar ) {
            CNWSMessage* message = CServerExoApp__GetNWSMessage((*NWN_AppManager)->app_server);
            CNWSMessage__SendServerToPlayerCCMessage(message, client_tar->pl_id, 13, msg, 0);
        }

        if ( msg ) {
            CNWCCMessageData_destroy(msg);
        }
    }

    combat.Log(3,
               "Touch Attack: ab: %d, ac: %d, conceal: %d, result: %d"
               " attack result: %d, roll: %d",
               ab, ac, conceal, result, attack_result, roll);

    if ( CVirtualMachine__StackPushInteger(*NWN_VirtualMachine, result) ) {
        return 0;
    }

    return -639;
}
