#ifndef NWNX_CPP_H
#define NWNX_CPP_H

#include <vector>
#include <string>
#include <unordered_map>

#include "NWNXBase.h"
#include "ScriptManager.h"
#include "core/pluginlink.h"

int Handle_PluginsLoaded(uintptr_t e);
int Handle_RunScript(uintptr_t p);
int Handle_RunScriptSituation(uintptr_t p);

class CNWNXCpp : public CNWNXBase
{
    std::string script_dir_;

public:
    CNWNXCpp();
    ~CNWNXCpp();

    std::string GetConf(const char* key);
    bool InitializeEventHandlers();
    bool OnCreate(gline *nwnxConfig, const char *LogDir = NULL);
    char* OnRequest(char* gameObject, char* Request, char* Parameters);
    //unsigned long OnRequestObject (char *gameObject, char* Request);
    bool OnRelease();
    void Initialize();

    ScriptManager sm;
};

#endif /* NWNX_CPP_H */
