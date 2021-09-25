#ifndef NWNX_CALLBACK_H
#define NWNX_CALLBACK_H

#include <vector>
#include <string>
#include <unordered_map>

#include "NWNXBase.h"
#include "core/pluginlink.h"
#include "api/all.h"

int Handle_PluginsLoaded(uintptr_t e);
int Handle_RunScriptSituation(uintptr_t p);

struct CallbackPayload {
    uint32_t object;
    std::string script;
    float delay;
    float step;
    float min;
    bool retain;
};

using CallbackMap = std::unordered_map<uint32_t, CallbackPayload>;

class CNWNXCallback : public CNWNXBase
{
    CallbackMap callbacks_;
public:
    CNWNXCallback();
    ~CNWNXCallback();

    std::string GetConf(const char* key);
    bool InitializeEventHandlers();
    bool OnCreate(gline *nwnxConfig, const char *LogDir = NULL);
    char* OnRequest(char* gameObject, char* Request, char* Parameters);
    unsigned long OnRequestObject (char *gameObject, char* Request);
    bool OnRelease();
    void Initialize();

    /**
     * @brief RegisterCallBack
     * @param object Object the script will be called on...
     * @param script Script to call...
     * @param delay Every N seconds...
     * @param step Optionally step by...
     * @param retain Retain even if object is invalid.
     * @return
     */
    bool RegisterCallback(uint32_t object, const char* script, float delay, float step = 0.0f, float min = 0.0f, bool retain = false);

    /**
     * @brief RunCallback
     * @param token Int token.
     * @return true if run, false if not.
     */
    bool RunCallback(uint32_t token);
};

#endif /* NWNX_CALLBACK_H */
