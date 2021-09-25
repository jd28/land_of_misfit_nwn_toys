/***************************************************************************
    Copyright (C) 2013 jmd (jmd2028 at gmail dot com)

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

#include "NWNXItems.h"

extern CNWNXItems items;

#define M(str, lit)                                                 \
    strncmp((str), "" lit, (sizeof (lit)/sizeof(char)) - 1) == 0

char * HandleRequest(CGameObject *ob, const char *request, char *value) {

    if( M(request, "GET_DEFAULT_ILR") ){
        if ( ob ) {
            sprintf(value, "%d", CNWSItem__GetMinEquipLevel(nwn_GetItemByID(ob->id)));
        }
        else {
            sprintf(value, "-1");
        }
    }
    else if( items.in_script ) {
        if( M(request, "GET_TYPE") ){
            sprintf(value, "%d", items.event.type);
        }
        else if( M(request, "SET_RESULT") )
        {
            items.event.use_result = true;
            items.event.result = atoi(value);
        }

    }
    else {
        if( M(request, "SET_ITEMS_HANDLER_") ) {
            int ev_num = atoi(request + 18);
	    
            if( ev_num < 0 || ev_num >= ITEMS_EVENT_NUM ) {
                items.Log(0,"ERROR: Invalid equip event number %d!\n", ev_num);
                sprintf(value, "-1");
                return NULL;
            }

            char buffer[17] = { 0 };
            strncpy(buffer, value, 16);

            if( ev_num == 0 ) {
                std::fill_n(items.event_scripts, ITEMS_EVENT_NUM, buffer);
            }
            else {
                items.event_scripts[ev_num] = buffer;
            }

            sprintf(value, "1");
        }
    }
    
    return NULL;
}

unsigned long HandleRequestObject(CGameObject *ob, const char *request) {
    if( items.in_script ) {
        if( M(request, "OBJECT") ){
            return items.event.object;
        }
        else if( M(request, "ITEM") ){
            return items.event.item;
        }
    }
    return OBJECT_INVALID;
}
