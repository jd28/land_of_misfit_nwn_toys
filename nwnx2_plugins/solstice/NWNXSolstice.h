/***************************************************************************
    Copyright (C) 2011 jmd ( jmd2028 at gmail dot com )

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

#ifndef NWNX_SOLSTICE_H
#define NWNX_SOLSTICE_H

#include <vector>

#include "NWNXLib.h"
#include "talib/nwn/all.h"
#include "consts.h"
#include "combat/dice.h"
#include "core/pluginlink.h"

struct EffectData {
    CNWSObject *obj;
    CGameEffect *eff;
    bool is_remove;
};

struct DamageResult {
    int32_t    damages[DAMAGE_INDEX_NUM];
    int32_t    damages_unblocked[DAMAGE_INDEX_NUM];
    int32_t    immunity[DAMAGE_INDEX_NUM];
    int32_t    resist[DAMAGE_INDEX_NUM];
    int32_t    resist_remaining[DAMAGE_INDEX_NUM];
    int32_t    reduction, reduction_remaining, parry;
};


#ifdef __cplusplus
extern "C" {
#endif
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "nwnx_modules_ext.h"
#include "lua_funcs.h"
#include "solstice_funcs.h"

bool hook_functions();

char* HandleRequest(CGameObject *ob, const char *request, char *value);

void              Local_NWNXLog(int level, const char* log);
void              Local_SetDamageInfo(int32_t index, const char* name, const char* color);
void              Local_SetCombatEngineActive(bool active);
EffectData       *Local_GetLastEffect();

#ifdef __cplusplus
}

#include "NWNXBase.h"
#include <string>

bool init_event_handles();

int Handle_PluginsLoaded(uintptr_t p);
int Handle_RunScript(uintptr_t p);
int Handle_RunScriptSituation(uintptr_t p);

class CNWNXSolstice : public CNWNXBase
{
public:
    CNWNXSolstice();
    ~CNWNXSolstice();

    std::string GetConf(const char* key);
    bool InitializeEventHandlers();
    bool OnCreate(gline *nwnxConfig, const char *LogDir = NULL);
    char* OnRequest(char* gameObject, char* Request, char* Parameters);
    //unsigned long OnRequestObject (char *gameObject, char* Request);
    bool OnRelease();
    void Initialize();

    bool bHooked;
    bool combat_engine_active = false;
    int            in_conditional_script;
    int            lua_last_return;
    uint32_t       lua_attacks = 0;
    uint32_t       nwn_attacks = 0;
    uint64_t       lua_time = 0;
    uint64_t       nwn_time = 0;
    bool           hook_nonstacks;
    uint32_t       updates = 0;
    uint64_t       update_time = 0;
    EffectData     last_effect;
    std::vector<std::string> damage_names;
    std::vector<std::string> damage_colors;
};
#endif

#endif /* NWNX_SOLSTICE_H */
