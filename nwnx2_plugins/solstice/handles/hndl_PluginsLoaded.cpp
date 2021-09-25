#include "NWNXSolstice.h"
#include "hooks.h"

extern CNWNXSolstice solstice;

#define HOOK(orig, addr, hook, bytes) \
    *(void**)&orig = nx_hook_function((void*)addr, (void*)hook, bytes, NX_HOOK_DIRECT | NX_HOOK_RETCODE)

int Handle_PluginsLoaded(uintptr_t p)
{
    if (!solstice.InitializeEventHandlers())
        solstice.Log(0, "ERROR: Unable to initialize NWNX event handlers!");

    HOOK(CNWSCreature__GetMaxHitPoints, 0x0812E25C, Hook_GetMaxHitpoints, 5);

    return 0;
}
