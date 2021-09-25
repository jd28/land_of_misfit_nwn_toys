/***************************************************************************
    NWNX Items
    (c) 2012-2013 jmd (jmd2028 at gmail dot com)

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

#ifndef _NWNX_ITEMS_H_
#define _NWNX_ITEMS_H_

#include "talib/nwn/all.h"
#include "NWNXLib.h"
#include "NWNXBase.h"
#include "pluginlink.h"

#include <string>

#define ITEMS_EVENT_ALL                  0
#define ITEMS_EVENT_CAN_EQUIP            1
#define ITEMS_EVENT_CAN_UNEQUIP          2
#define ITEMS_EVENT_MIN_LEVEL            3
#define ITEMS_EVENT_CAN_USE              4
#define ITEMS_EVENT_CALC_BASE_COST       5
#define ITEMS_EVENT_COMPUTE_WEIGHT       6
#define ITEMS_EVENT_NUM                  7

const int ITEMS_CURRENT_IP_VERSION = 0;

bool hook_functions();

char * HandleRequest(CGameObject *ob, const char *request, char *value);
unsigned long HandleRequestObject(CGameObject *ob, const char *request);

bool Local_GetIsHelmetHidden(CNWSCreature *cre);

class CNWNXItems : public CNWNXBase {
    HANDLE hItemEvent;

public:
    CNWNXItems();
    ~CNWNXItems();
    bool OnCreate(gline *config, const char* LogDir);
    const char* GetConf(char *key);
    char* OnRequest(char* gameObject, char* Request, char* Parameters);
    unsigned long OnRequestObject (char *gameObject, char* Request);
    bool OnRelease();

    void FireEvent(int32_t type, uint32_t obj_id, uint32_t item_id);

    std::string event_scripts[ITEMS_EVENT_NUM];
    bool in_script;
    ItemsInfoEvent event;
    bool override_proficiencies = false;
};

#endif
