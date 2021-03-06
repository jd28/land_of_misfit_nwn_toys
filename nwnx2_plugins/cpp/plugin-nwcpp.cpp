#include "NWNXCpp.h"

CNWNXCpp nwcpp;
PLUGINLINK *pluginLink = 0;

PLUGININFO pluginInfo = {
    sizeof(PLUGININFO),
    "NWNXCpp",
    PLUGIN_MAKE_VERSION(0, 0, 0, 1),
    "",
    "jmd",
    "jmd2028@gmail.com",
    "(c) 2011-current jmd",
    "",
    0		//not transient
};


extern "C" PLUGININFO* GetPluginInfo(DWORD nwnxVersion)
{
    return &pluginInfo;
}

extern "C" int InitPlugin(PLUGINLINK *link)
{
    pluginLink = link;
    return 0;
}


extern "C" CNWNXBase* GetClassObject()
{
    return &nwcpp;
}
