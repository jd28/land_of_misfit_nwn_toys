#include "NWNXCallback.h"

extern CNWNXCallback callback;

int Handle_RunScriptSituation(uintptr_t p)
{
    CoreRunScriptSituationEvent *s = reinterpret_cast<CoreRunScriptSituationEvent *>(p);

    if (!s || !s->marker) {
        return 0;
    }

    callback.Log(4, "Script names: %s\n", s->marker);

    if (strcmp(s->marker, "$callback") != 0) {
        callback.Log(4, "This situation isn't for us.\n");
        return 0;
    }

    if (callback.RunCallback(s->token)) {
        s->suppress = true;
        return 1;
    }

    return 0;
}
