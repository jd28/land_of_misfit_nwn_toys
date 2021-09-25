#include "nx_devel.h"
#include "nx_log.h"
#include "api/all.h"
#include "ScriptLib.h"

static const int EVENT_TIMED_EVENT = 1;
const nwobjid OBJECT_SELF = 0xFFFFFFFF;
const nwobjid OBJECT_INVALID = 0x7F000000;

static CVirtualMachineScript *CreateFakeScript(const char *marker, uint32_t token) {
    CVirtualMachineScript *vms = (CVirtualMachineScript *) malloc(sizeof(CVirtualMachineScript));
    vms->Name.Text = strdup(marker);
    vms->Stack = NULL;
    vms->Code = NULL;
    vms->StackSize = token;
    return vms;
}

void nwn_DelayCommand(nwobjid objid, float delay, const char* marker, uint32_t token)
{
    CVirtualMachineScript *vms = CreateFakeScript(marker, token);
    uint32_t day, time;
    CWorldTimer *timer = reinterpret_cast<CWorldTimer *>(g_pAppManager->ServerExoApp->Internal->WorldTimer1);
    day = timer->GetCalendarDayFromSeconds(delay);
    time = timer->GetTimeOfDayFromSeconds(delay);

    CServerAIMaster *ai_master = reinterpret_cast<CServerAIMaster *>(g_pAppManager->ServerExoApp->Internal->m_ServerAIMaster);
    int result = ai_master->AddEventDeltaTime(day, time, objid, objid, EVENT_TIMED_EVENT, vms);

    if(result == 0) {
        nx_log(NX_LOG_ERROR, 0, "DelayCommand Error: %d", result);
    }

    nx_log(NX_LOG_INFO, 3, "id: %x, delay: %.2f, token: %d, scriptsit: %s\n",
           objid, delay, token, marker);

}

void nwn_ActionDoCommand(nwobjid objid, const char* marker, uint32_t token)
{
    CGameObject *obj = g_pAppManager->ServerExoApp->GetGameObject(objid);
    if (!obj) {
        nx_log(NX_LOG_ERROR, 0, "ERROR: ActionDoCommand does not exist: %d", objid);
        return;
    }

    // Areas don't work in this context.
    if(obj->ObjectType == 4) {
        return;
    }

    CNWSObject *object = (CNWSObject*)obj;
    if(!object->CanModifyActionQueue) {
        return;
    }

    CVirtualMachineScript *vms = CreateFakeScript(marker, token);
    object->AddAction(37, 65535, 5, vms, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0);
}

void nwn_ExecuteCommand(int command, int num_args)
{
    g_pVirtualMachine->m_pCmdImplementer->ExecuteCommand(command, num_args);
}

nwobjid nwn_GetCommandObjectId()
{
    return g_pVirtualMachine->m_pCmdImplementer->ObjectID;
}

nwobjid nwn_SetCommandObjectId(nwobjid obj)
{
    return g_pVirtualMachine->m_pCmdImplementer->ObjectID = obj;
}

int32_t nwn_StackPopInteger()
{
    int32_t result = 0;
    int32_t err = g_pVirtualMachine->StackPopInteger(&result);
    nx_log(NX_LOG_ERROR, 3, "StackPopInteger Error: %d, result: %d", err, result);
    return result;
}

float nwn_StackPopFloat()
{
    float result = 0.0;
    g_pVirtualMachine->StackPopFloat(&result);
    return result;
}

char * nwn_StackPopString()
{
    CExoString str = { NULL, 0 };

    int err = g_pVirtualMachine->StackPopString(&str);
    nx_log(NX_LOG_INFO, 3, "StackPopString Error: %d", err);
    return strdup(str.Text);
}

Vector *nwn_StackPopVector()
{
    Vector *buf = (Vector *)malloc(sizeof(Vector));
    g_pVirtualMachine->StackPopVector(buf);

    return buf;
}

nwobjid nwn_StackPopObject()
{
    unsigned long result;
    g_pVirtualMachine->StackPopObject(&result);
    return result;
}

void * nwn_StackPopEngineStructure(uint32_t type)
{
    void *result = NULL;

    if(!g_pVirtualMachine->StackPopEngineStructure(type, &result))
        return NULL;

    return result;
}

void nwn_StackPushInteger(int32_t value)
{
    g_pVirtualMachine->StackPushInteger(value);
}

void nwn_StackPushFloat(float value)
{
    g_pVirtualMachine->StackPushFloat(value);
}

void nwn_StackPushString(const char *value)
{
    CExoString cs(value);
    g_pVirtualMachine->StackPushString(cs);
}

void nwn_StackPushVector(Vector *value)
{
    g_pVirtualMachine->StackPushVector(*value);
}

void nwn_StackPushObject(nwobjid value)
{
    // This is a bit of a hack probably...
    nwobjid obj = value;
    if(value == OBJECT_SELF) {
        obj = 0x7f000000;
        uint32_t rec = g_pVirtualMachine->RecursionLevel;
        if(g_pVirtualMachine->m_bValidObjectRunScript[rec]) {
            obj = g_pVirtualMachine->ObjectID[rec];
        }
    }

    g_pVirtualMachine->StackPushObject(obj);
}

void nwn_StackPushEngineStructure(uint32_t type, void * value)
{
    g_pVirtualMachine->StackPushEngineStructure(type, value);
}
