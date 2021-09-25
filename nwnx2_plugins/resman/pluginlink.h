#pragma once

typedef struct {
    const char* file;
} ResManAddContainerEvent;

#define EVENT_RESMAN_ADD_CONTAINER "ResMan/AddContainer"

typedef struct {
    void *container;
    bool is_static;
} ResManAddContainerService;

#define SERVICE_RESMAN_ADD_CONTAINER "ResMan/AddContainer()"
