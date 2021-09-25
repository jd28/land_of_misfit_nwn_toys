#ifndef NWNX_SCRIPT_LIB_H
#define NWNX_SCRIPT_LIB_H

#include <stdint.h>
#include "../typedef.h"

#define ENGINE_NUM_STRUCTURES           5
#define ENGINE_STRUCTURE_EFFECT         0
#define ENGINE_STRUCTURE_EVENT          1
#define ENGINE_STRUCTURE_LOCATION       2
#define ENGINE_STRUCTURE_TALENT         3
#define ENGINE_STRUCTURE_ITEMPROPERTY   4

class Vector;

extern const nwobjid OBJECT_SELF;
extern const nwobjid OBJECT_INVALID;

extern "C" {
    /**
     * @brief Create a new delayed command.
     * @param objid Object running the command.
     * @param delay Number of seconds to delay the command by.
     * @param marker Plugin specific marker, for determing if it should run the command.
     * @param token Plugin specific integer token for determining which command to run.
     */
    void      nwn_DelayCommand(nwobjid objid, float delay, const char* marker, uint32_t token);

    /**
     * @brief Create a new DoCommand action.
     * @param objid Object running the command.
     * @param marker Plugin specific marker, for determing if it should run the command.
     * @param token Plugin specific integer token for determining which command to run.
     */
    void      nwn_ActionDoCommand(nwobjid objid, const char* marker, uint32_t token);

    /**
     * @brief nwn_ExecuteCommand
     * @param command
     * @param num_args
     */
    void      nwn_ExecuteCommand(int command, int num_args);

    /**
     * @brief Gets current command object.
     * @return Object ID that is OBJECT_SELF.
     */
    nwobjid   nwn_GetCommandObjectId();
    nwobjid   nwn_SetCommandObjectId(nwobjid obj);

    int32_t   nwn_StackPopInteger();
    float     nwn_StackPopFloat();
    /**
     * @brief Pops string off the stack.  Caller is responsible for freeing.
     * @return Null terminated string.
     */
    char     *nwn_StackPopString();
    /**
     * @brief Pops Vector from the stack.  Caller is responsible for freeing.
     * @return
     */
    Vector   *nwn_StackPopVector();
    nwobjid nwn_StackPopObject();
    /**
     * @brief Pops engine structure.  Caller is responsible for freeing.
     * @param type ENGINE_STRUCTURE_* constant.
     * @return
     */
    void     *nwn_StackPopEngineStructure(uint32_t type);
    void      nwn_StackPushFloat(float value);
    void      nwn_StackPushInteger(int32_t value);
    void      nwn_StackPushString(const char *value);
    void      nwn_StackPushVector(Vector *value);
    void      nwn_StackPushObject(nwobjid value);
    void      nwn_StackPushEngineStructure(uint32_t type, void * value);
}

#endif // NWNX_SCRIPT_LIB_H
