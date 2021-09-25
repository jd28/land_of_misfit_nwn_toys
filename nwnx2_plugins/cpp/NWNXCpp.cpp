#include "NWNXCpp.h"
#include "ScriptManager.h"

extern PLUGINLINK *pluginLink;

CNWNXCpp::CNWNXCpp()
{
    confKey = strdup("CPP");
    script_dir_ = "scripts";
}

CNWNXCpp::~CNWNXCpp()
{
    OnRelease();
}

std::string CNWNXCpp::GetConf(const char* key)
{
    return (*nwnxConfig)[confKey][key];
}

bool CNWNXCpp::OnCreate(gline *config, const char *LogDir)
{
    char log[128];
    sprintf(log, "%s/nwnx_cpp.txt", LogDir);


    // call the base class function
    if(!CNWNXBase::OnCreate(config, log))
        return false;

    // Plugin Events
    if(!pluginLink) {
        Log(0, "Plugin link not accessible\n");
        return false;
    } else {
        Log(0, "Plugin link: %08lX\n", pluginLink);
    }


    HANDLE handlePluginsLoaded = HookEvent(EVENT_CORE_PLUGINSLOADED, Handle_PluginsLoaded);

    if(!handlePluginsLoaded) {
        Log(0, "Cannot hook plugins loaded event!\n");
        return false;
    }

    Log(0, "NWNX Cpp v0.1\n");
    Log(0, "(c) by jmd (jmd2028 at gmail dot com, 2015)\n");

    Log(0, "* Module loaded successfully.\n");
    return true;
}

void CNWNXCpp::Initialize()
{
}

char* CNWNXCpp::OnRequest(char *gameObject, char* Request, char* Parameters)
{
    Log(2, "(S) Request: \"%s\"\n", Request);
    Log(3, "(S) Params:  \"%s\"\n", Parameters);

    if(!gameObject) {
        return NULL;
    }

    return NULL;
}

bool CNWNXCpp::OnRelease()
{
    return true;
}

bool CNWNXCpp::InitializeEventHandlers()
{
    Log(0, "Initializing Event Handlers\n");
    bool result = true;
    HANDLE handleRunScriptEvent = HookEvent(EVENT_CORE_RUNSCRIPT, Handle_RunScript);

    if(!handleRunScriptEvent) {
        Log(0, "Cannot hook NWServer/RunScript!\n");
    }

    HANDLE handleRunScriptSituationEvent = HookEvent(EVENT_CORE_RUNSCRIPT_SITUATION,
                                           Handle_RunScriptSituation);

    if(!handleRunScriptSituationEvent) {
        Log(0, "Cannot hook NWServer/RunScriptSituation!\n");
    }

    sm.LoadScripts(script_dir_);

    return result;
}

