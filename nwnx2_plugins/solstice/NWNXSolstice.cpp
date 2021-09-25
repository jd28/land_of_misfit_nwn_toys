/***************************************************************************
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

#include "NWNXSolstice.h"

static int dolibrary(lua_State *L, const char *name)
{
    lua_getglobal(L, "require");
    lua_pushstring(L, name);
    return lua_pcall(L, 1, 0, 0);
}

extern PLUGINLINK *pluginLink;

lua_State *L = NULL;

CNWNXSolstice::CNWNXSolstice()
{
    confKey = strdup("SOLSTICE");
    bHooked = 1;
    lua_last_return = -1;
    talib_init();
    damage_names.resize(DAMAGE_INDEX_NUM, "");
    damage_colors.resize(DAMAGE_INDEX_NUM, "");
}

CNWNXSolstice::~CNWNXSolstice()
{
    OnRelease();
}

std::string CNWNXSolstice::GetConf(const char* key)
{
    return (*nwnxConfig)[confKey][key];
}

bool CNWNXSolstice::OnCreate(gline *config, const char *LogDir)
{
    char log[128];
    sprintf(log, "%s/nwnx_solstice.txt", LogDir);


    // call the base class function
    if (!CNWNXBase::OnCreate(config, log))
        return false;

    // Plugin Events
    if (!pluginLink) {
        Log(0, "Plugin link not accessible\n");
        return false;
    } else {
        Log(0, "Plugin link: %08lX\n", pluginLink);
    }

    last_effect.obj = nullptr;
    last_effect.eff = nullptr;

    HANDLE handlePluginsLoaded = HookEvent(EVENT_CORE_PLUGINSLOADED, Handle_PluginsLoaded);
    if (!handlePluginsLoaded) {
        Log(0, "Cannot hook plugins loaded event!\n");
        return false;
    }

    Log(0, "NWNX Solstice v0.2\n");
    Log(0, "(c) by jmd (jmd2028 at gmail dot com, 2011-2012)\n");
    hook_nonstacks = atoi(GetConf("hook_nonstack_effects").c_str());
    Log(0, "Hooking non-stacking effects: %d\n", hook_nonstacks);

    L = lua_open();
    luaL_openlibs(L);
    bHooked = hook_functions();

    Log(0, "* Module loaded successfully.\n");
    return true;
}

void CNWNXSolstice::Initialize()
{
    std::string dir = GetConf("script_dir");
    if (dir.empty()) {
        dir = "solstice/preload";
    } else {
        dir += "/preload";
    }

    int status = dolibrary(L, dir.c_str());
    if (status) {
        /* If something went wrong, error message is at the top of */
        /* the stack */
        Log(0, "Couldn't load file: %s\n", lua_tostring(L, -1));
    }
}

char* CNWNXSolstice::OnRequest(char *gameObject, char* Request, char* Parameters)
{
    Log(2, "(S) Request: \"%s\"\n", Request);
    Log(3, "(S) Params:  \"%s\"\n", Parameters);

    if (!gameObject) { return NULL; }

    HandleRequest(reinterpret_cast<CGameObject*>(gameObject),
                  Request, Parameters);

    return NULL;
}

bool CNWNXSolstice::OnRelease()
{
    if (lua_attacks > 0)
        Log(0, "Average Lua Melee Attack Time: %d\n", lua_time / lua_attacks);

    if (nwn_attacks > 0)
        Log(0, "Average NWN Melee Attack Time: %d\n", nwn_time / nwn_attacks);

    if (updates > 0)
        Log(0, "Average Update Time: %d\n", update_time / updates);

    int nKBytes = lua_gc(L, LUA_GCCOUNT, 0);
    Log(0, "o Shutdown.. Memory: %d Kb\n", nKBytes);

    return true;
}

bool CNWNXSolstice::InitializeEventHandlers()
{
    bool result = true;

#define EVENT(name, func) \
    if (!HookEvent(name, func)) { \
        Log(0, "Cannot hook %s!\n", name); \
        result = false; \
    }

    EVENT(EVENT_CORE_RUNSCRIPT, Handle_RunScript);
    EVENT(EVENT_CORE_RUNSCRIPT_SITUATION, Handle_RunScriptSituation);

    return result;
#undef EVENT
}
