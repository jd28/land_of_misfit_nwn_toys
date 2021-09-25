#ifndef NWNX_RESMAN_H
#define NWNX_RESMAN_H

#include "NWNXBase.h"
#include "NwnDefines.h"
#include "api/all.h"

#include "resources/ResourceManager.h"

constexpr size_t MAXPATH = 512;

int HookFunctions();
void HookResman();

class CNWNXResMan : public CNWNXBase
{
    void WriteLogHeader();
    void LoadConfiguration();

public:
    CNWNXResMan();
    virtual ~CNWNXResMan();
    bool OnCreate(gline *config, const char* LogDir);
    char* OnRequest(char* gameObject, char* Request, char* Parameters);
    bool OnRelease();

    void DumpResStruct(CRes *cRes);
    CKeyTableEntry *CreateKeyTableEntry(const CResRef& resRef, NwnResType resType);
    bool debugCRes;

    bool AddContainer(const char* path, const char* name);

    ResourceManager resources;

private:
    char *pScriptBuffer;
    bool warnMissing;
    HANDLE hAddContainer;
    HANDLE hAddContainerService;
};

#endif // NWNX_RESMAN_H
