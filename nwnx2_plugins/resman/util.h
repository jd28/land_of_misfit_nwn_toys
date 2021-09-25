#pragma once

#include "api/all.h"
#include "NwnDefines.h"
#include <cstring>

static char resref_to_fname_buffer[21];
inline const char* resref_to_fname(const CResRef& resref, uint16_t type) {
    memset(resref_to_fname_buffer, 0, 21);
    const char* resExt = NwnGetResTypeExtension((NwnResType)type);
    strncat(resref_to_fname_buffer, resref.m_resRef, 16);
    strcat(resref_to_fname_buffer, ".");
    if(!resExt) {
        return "";
        //Log(0, "Invalid Res Type: %d\n", type);
    }
    strcat(resref_to_fname_buffer, resExt);
    return resref_to_fname_buffer;
}
