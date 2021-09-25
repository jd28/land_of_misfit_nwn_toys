#include "NWNXSolstice.h"
#include "hooks.h"

extern CNWNXSolstice solstice;

int Hook_LoadModuleStart(CNWSModule *mod, void *a2)
{
    int res = CNWSModule__LoadModuleStartNext(mod, a2);
    solstice.Initialize();
    return res;
}
