#include "ScriptLib.h"
#include "ScriptManager.h"
#include "types.h"
#include "nx_log.h"
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <dlfcn.h>

typedef void (*registerScriptsPtr)(ScriptManager *);

std::unordered_map<unsigned long, std::function<void()> > ScriptManager::situations;

bool ScriptManager::RegisterScript(std::string name, MainScript script)
{
    ScriptVariant v;
    v.is_conditional = false;
    v.payload.main = script;
    v.so = loading_so_;
    auto result = scripts_.insert({name, v});
    return result.second;
}

bool ScriptManager::RegisterConditionalScript(std::string name, ConditionalScript script)
{
    ScriptVariant v;
    v.is_conditional = true;
    v.payload.conditional = script;
    v.so = loading_so_;
    auto result = scripts_.insert({name, v});
    return result.second;
}

bool ScriptManager::HasScript(const std::string& script)
{
    return scripts_.find(script) != scripts_.end();
}

int ScriptManager::RunScript(const std::string &script, uint32_t obj)
{
    auto it = scripts_.find(script);

    if(it == scripts_.end()) {
        return -1;
    }

    ScriptVariant v = (*it).second;
    int result = -1;

    nx_log(NX_LOG_INFO, 0, "Running script %s on %d", script.c_str(), obj);

    if(v.is_conditional) {
        result = v.payload.conditional(obj);
    } else {
        v.payload.main(obj);
    }

    return result;
}

bool ScriptManager::LoadScripts(const std::string &dir)
{
    fs::path p(dir);
    fs::directory_iterator end_iter;

    void *handle;

    if(fs::exists(p) && fs::is_directory(p)) {
        fs::directory_iterator dir_iter(p);

        if(dir_iter == end_iter) {
            nx_log(NX_LOG_ERROR, 0, "No files in director: %s", dir.c_str());
        }

        for(; dir_iter != end_iter ; ++dir_iter) {
            std::string file = (*dir_iter).path().string();
            std::string stem = (*dir_iter).path().stem().string();
            nx_log(NX_LOG_INFO, 0, "Inspecting file: %s", file.c_str());

            if(fs::is_regular_file(dir_iter->status())) {
                if(boost::starts_with(stem, "nwcpp_")) {
                    // load the library
                    if((handle = dlopen(file.c_str(), RTLD_NOW)) == NULL) {
                        nx_log(NX_LOG_ERROR, 0, "ERROR: dlopen: %s: %s\n", file.c_str(), dlerror());
                        continue;
                    }

                    registerScriptsPtr registerScripts;

                    // get the Class Accessor
                    if((registerScripts = (registerScriptsPtr)dlsym(handle, "registerScripts")) == NULL) {
                        nx_log(NX_LOG_ERROR, 0, "ERROR: dlsym: %s", dlerror());
                        dlclose(handle);
                        continue;
                    }

                    loading_so_ = file;
                    nx_log(NX_LOG_INFO, 0, "Loading from %s", file.c_str());
                    registerScripts(this);
                    loading_so_ = "";
                }
            }
        }
    } else {
        nx_log(NX_LOG_ERROR, 0, "Directory %s does not exist!\n", dir.c_str());
        return false;
    }

    return true;
}

void ScriptManager::RemoveLibrary(const std::string &so)
{
    // This is probably not safe if there are script situations in flight.
    for(auto it = scripts_.begin(); it != scripts_.end();) {
        if(it->second.so == so) {
            it = scripts_.erase(it);
        } else {
            ++it;
        }
    }
}

unsigned long ScriptManager::RegisterScriptSituation(const std::function<void ()>& func)
{
    static unsigned long current_token = 0;

    unsigned long token = current_token++;
    situations.emplace(std::pair<unsigned long, std::function<void()>>(token, func));
    return token;
}

bool ScriptManager::RunScriptSituation(unsigned long token, unsigned long objectId)
{
    auto it = situations.find(token);

    if(it == situations.end()) {
        nx_log(NX_LOG_INFO, 0, "No Scriptsituation %d", token);
        return false;
    }

    unsigned long temp = nwn_GetCommandObjectId();
    nwn_SetCommandObjectId(objectId);
    (*it).second();
    nwn_SetCommandObjectId(temp);
    situations.erase(token);
    return true;
}
