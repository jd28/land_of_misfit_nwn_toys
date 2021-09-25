#include "NWNXCpp.h"

extern CNWNXCpp nwcpp;

int Handle_PluginsLoaded(uintptr_t e)
{
    if(!nwcpp.InitializeEventHandlers())
        nwcpp.Log(0, "ERROR: Unable to initialize NWNX event handlers!");

    return 0;
}
