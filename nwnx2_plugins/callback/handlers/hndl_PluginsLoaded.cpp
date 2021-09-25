#include "NWNXCallback.h"

extern CNWNXCallback callback;

int Handle_PluginsLoaded(uintptr_t e)
{
    if (!callback.InitializeEventHandlers())
        callback.Log(0, "ERROR: Unable to initialize NWNX event handlers!");

    return 0;
}
