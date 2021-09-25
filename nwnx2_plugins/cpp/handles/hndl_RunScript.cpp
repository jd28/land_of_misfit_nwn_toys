#include "NWNXCpp.h"
#include "api/all.h"

extern CNWNXCpp nwcpp;

int Handle_RunScript(uintptr_t p)
{
    CoreRunScriptEvent *s = reinterpret_cast<CoreRunScriptEvent *>(p);

    if(!s) {
        nwcpp.Log(0, "No core event!\n");
    }

    nwcpp.Log(0, "NWCPP: Script Name: |%s| on %d\n", s->resref, s->objectId);

    if(!nwcpp.sm.HasScript(s->resref)) {
        nwcpp.Log(0, "Script %s not found!\n", s->resref);
        return 0;
    }
    int rec = g_pVirtualMachine->RecursionLevel;
    g_pVirtualMachine->ObjectID[rec] = s->objectId;
    g_pVirtualMachine->m_bValidObjectRunScript[rec] = s->recursionLevel;
    g_pVirtualMachine->m_pCmdImplementer->ObjectID = s->objectId;

    s->suppress = true;
    s->returnValue = nwcpp.sm.RunScript(s->resref, s->objectId);

    return 1;
}
