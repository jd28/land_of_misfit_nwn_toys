#include "NWNXSolstice.h"

extern CNWNXSolstice solstice;

void Local_NWNXLog(int level, const char* log)
{
    solstice.Log(level, log);
}
