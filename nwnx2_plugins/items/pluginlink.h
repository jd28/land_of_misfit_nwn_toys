#pragma once

#include <stdint.h>
#include "../typedef.h"

#define EVENT_ITEMS_INFO "Items/Info"
typedef struct {
    int       type;
    nwobjid   object;
    nwobjid   item;
    bool      use_result;
    uint32_t  result;
} ItemsInfoEvent;
