#include "NWNXCallback.h"
#include "api/all.h"
#include "nx_log.h"

/* temporary copy and paste */

static const int EVENT_TIMED_EVENT = 1;

static CVirtualMachineScript *CreateFakeScript(const char *marker, uint32_t token)
{
    CVirtualMachineScript *vms = (CVirtualMachineScript *) malloc(sizeof(CVirtualMachineScript));
    vms->Name.Text = strdup(marker);
    vms->Stack = NULL;
    vms->Code = NULL;
    vms->StackSize = token;
    return vms;
}

static void nwn_DelayCommand(nwobjid objid, float delay, const char* marker, uint32_t token)
{
    CVirtualMachineScript *vms = CreateFakeScript(marker, token);
    uint32_t day, time;
    CWorldTimer *timer = reinterpret_cast<CWorldTimer *>(g_pAppManager->ServerExoApp->Internal->WorldTimer1);
    day = timer->GetCalendarDayFromSeconds(delay);
    time = timer->GetTimeOfDayFromSeconds(delay);

    CServerAIMaster *ai_master = reinterpret_cast<CServerAIMaster *>(g_pAppManager->ServerExoApp->Internal->m_ServerAIMaster);
    int result = ai_master->AddEventDeltaTime(day, time, objid, objid, EVENT_TIMED_EVENT, vms);

    if (result == 0) {
        nx_log(NX_LOG_ERROR, 0, "DelayCommand Error: %d", result);
    }

    nx_log(NX_LOG_INFO, 3, "id: %x, delay: %.2f, token: %d, scriptsit: %s\n",
           objid, delay, token, marker);

}

/* end temporary copy and paste */

CNWNXCallback::CNWNXCallback()
{
    confKey = "CALLBACK";
}
CNWNXCallback::~CNWNXCallback() {}

std::string CNWNXCallback::GetConf(const char* key)
{
    return (*nwnxConfig)[confKey][key];
}

bool CNWNXCallback::InitializeEventHandlers()
{
    if (!HookEvent(EVENT_CORE_RUNSCRIPT_SITUATION, Handle_RunScriptSituation)) {
        Log(0, "Cannot hook EVENT_CORE_RUNSCRIPT_SITUATION!\n");
        return false;
    }

    return true;
}

bool CNWNXCallback::OnCreate(gline *nwnxConfig, const char *LogDir)
{
    char log[128];
    sprintf(log, "%s/nwnx_callback.txt", LogDir);

    // call the base class function
    if (!CNWNXBase::OnCreate(nwnxConfig, log))
        return false;

    // Plugin Events
    if (!pluginLink) {
        Log(0, "Plugin link not accessible\n");
        return false;
    } else {
        Log(0, "Plugin link: %08lX\n", pluginLink);
    }

    if (!HookEvent(EVENT_CORE_PLUGINSLOADED, Handle_PluginsLoaded)) {
        Log(0, "Cannot hook plugins loaded event!\n");
        return false;
    }

    return true;
}

char* CNWNXCallback::OnRequest(char* gameObject, char* Request, char* Parameters)
{
    if (!gameObject) {
        snprintf(Parameters, strlen(Parameters), "%d", 0);
        return NULL;
    }

    int result = 0;
    CGameObject *obj = reinterpret_cast<CGameObject*>(gameObject);
    uint32_t object = obj->ObjectID;

    if (strncmp("REGISTER", Request, 8) == 0) {
        char script[17] = {0};
        float delay;
        float step;
        float min;
        int retain;

        if (sscanf(Parameters, "%f~%f~%f~%d~%s", &delay, &step, &min, &retain, script) == 5
                && RegisterCallback(object, script, delay, step, min, retain)) {
            result = 1;
        }
    }
    snprintf(Parameters, strlen(Parameters), "%d", result);
    return NULL;
}

unsigned long CNWNXCallback::OnRequestObject(char *gameObject, char* Request)
{
    // Maybe stuff here...
    return OBJECT_INVALID;
}

bool CNWNXCallback::OnRelease()
{
    return true;
}

bool CNWNXCallback::RegisterCallback(uint32_t object, const char* script, float delay, float step, float min, bool retain)
{
    static uint32_t token_id = 0;
    uint32_t current = token_id++;

    auto it = callbacks_.insert({
        current,
        CallbackPayload {
            object,
            script,
            delay,
            step,
            min,
            retain
        }
    });
    if (it.second) {
        Log(3, "Registering callback: Object: 0x%x Script: '%s' Delay: %f, Step: %f, Min: %f, Retain: %d\n",
            object, script, delay, step, min, retain);

        nwn_DelayCommand(object, delay, "$callback", current);
    }

    return it.second;
}

bool CNWNXCallback::RunCallback(uint32_t token)
{
    auto it = callbacks_.find(token);
    if (it == callbacks_.end())
        return false;

    Log(3, "Running Callback: %d\n", token);

    bool runscript = true;
    CallbackPayload& p = it->second;
    if (!g_pAppManager->ServerExoApp->GetGameObject(p.object)) {
        if (!p.retain) {
            callbacks_.erase(it);
        }
        runscript = false;
    }

    if (runscript) {
        CExoString script(p.script.c_str());
        g_pVirtualMachine->RunScript(&script, p.object, 1);
        if (p.step != 0.0f) {
            p.delay += p.step;
        }
        if (p.min != 0.0f) {
            p.delay = std::max(p.delay, p.min);
        }
    }

    if (p.delay > 0.0f) {
        // Could reuse the vms if that was available.
        nwn_DelayCommand(p.object, p.delay, "$callback", it->first);
    } else {
        callbacks_.erase(it);
    }

    return true;
}

