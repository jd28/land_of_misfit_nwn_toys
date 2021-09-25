#include "NWNXEvents.h"

extern CNWNXEvents events;

void Hook_SetPVPPlayerLikesMe(CNWSCreature *cre, nwn_objid_t vs, int state, int a) {
    if ( !cre || vs == OBJECT_INVALID ) { return; }

    events.Log(3, "SetPVPPlayerLikesMe: Object: %X, Versus: %X, State: %d, a: %d\n",
	       cre->obj.obj_id, vs, state, a);

    events.FireEvent(cre->obj.obj_id, 
		     EVENT_TYPE_PVP_STATE,
		     state,
		     vs);

    if ( events.event.bypass ) { return; }

    if ( cre->cre_pvp ) {
	for ( uint32_t i = 0; i < cre->cre_pvp->len; ++i ) {
	    CNWSPVPEntry& entry = cre->cre_pvp->data[i];
	    if ( entry.pvp_obj_id == vs ) {
		entry.pvp_like = state;
		break;
	    }
	}
    }

    CNWSCreature *cre_vs = nwn_GetCreatureByID(vs);

    if ( a == 1 && cre_vs && cre_vs->cre_pvp ) {
	for ( uint32_t i = 0; i < cre_vs->cre_pvp->len; ++i ) {
	    CNWSPVPEntry& entry = cre_vs->cre_pvp->data[i];
	    if ( entry.pvp_obj_id == vs ) {
		entry.pvp_like = state;
		break;
	    }
	}	
    }

    if ( state == 1 && cre_vs && !events.suppressForgetAggression ) {
	CNWSCreature__SetAssociatesToForgetAggression(cre_vs, cre->obj.obj_id);	
    }
}
