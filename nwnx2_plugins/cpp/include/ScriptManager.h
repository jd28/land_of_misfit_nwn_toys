#ifndef TALIB_SCRIPTS_SCRIPT_MANAGER_H
#define TALIB_SCRIPTS_SCRIPT_MANAGER_H

#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include <stdint.h>

typedef void (*MainScript)(unsigned long);
typedef int (*ConditionalScript)(unsigned long);

struct ScriptVariant {
    bool is_conditional;
    union {
        MainScript main;
        ConditionalScript conditional;
    } payload;
    std::string so;
};

struct ScriptManager {
private:
    std::unordered_map<std::string, ScriptVariant> scripts_;
    std::unordered_map<std::string, std::string> script_to_so_;
    std::string loading_so_;

public:
    static std::unordered_map<unsigned long, std::function<void()> > situations;
    static unsigned long RegisterScriptSituation(const std::function<void()> &func);
    static bool RunScriptSituation(unsigned long token, unsigned long objectId);

    bool RegisterScript(std::string, MainScript script);
    bool RegisterConditionalScript(std::string, ConditionalScript script);
    bool HasScript(const std::string& script);
    int  RunScript(const std::string& script, uint32_t obj);
    bool LoadScripts(const std::string& dir);
    void RemoveLibrary(const std::string& so);
};

#endif // TALIB_SCRIPTS_SCRIPT_MANAGER_H
