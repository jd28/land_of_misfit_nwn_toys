#include "NWNXCpp.h"

extern CNWNXCpp nwcpp;

int Handle_RunScriptSituation(uintptr_t p)
{
    CoreRunScriptSituationEvent *s = reinterpret_cast<CoreRunScriptSituationEvent *>(p);

    if(!s || !s->marker) {
        return 0;
    }

    if(strcmp(s->marker, "$nwcpp") != 0) {
        return 0;
    }
    else {
        nwcpp.Log(4, "This situation isn't for us.\n");
    }

    if(ScriptManager::RunScriptSituation(s->token, s->objectId)) {
        s->suppress = true;
        return 1;
    }

    return 0;
}
