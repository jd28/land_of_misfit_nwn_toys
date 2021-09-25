#include "types.h"

int (*CGameEffect__ctor)(CGameEffect *, int) = NULL;
int (*CGameEffect__dtor)(CGameEffect *, char a) = NULL;
int (*CScriptEvent__ctor)(CScriptEvent *) = NULL;
int (*CScriptEvent__dtor)(CScriptEvent *, char a) = NULL;
int (*CGameEffect__CopyEffect)(CGameEffect *, CGameEffect *, int32_t) = NULL;
int (*CScriptEvent__CopyScriptEvent)(CScriptEvent *, CScriptEvent *) = NULL;

static int inited = 0;

static void init_ptrs()
{
    inited = 1;
#define SET(nm, addr)                           \
    if ( nm == NULL ) *(int*)& nm = addr

    SET(CGameEffect__ctor, 0x0817DC70);
    SET(CGameEffect__dtor, 0x0817DFDC);
    SET(CGameEffect__CopyEffect, 0x0817E08C);
    SET(CScriptEvent__CopyScriptEvent, 0x08061164);
    SET(CScriptEvent__ctor, 0x080610BC);
    SET(CScriptEvent__dtor, 0x0806189C);

#undef SET
}


effect::effect()
{
    if(!inited) {
        init_ptrs();
    }

    structure = (CGameEffect *)malloc(sizeof(CGameEffect));
    CGameEffect__ctor(structure, 0);
}


effect::effect(void *s)
    : structure(reinterpret_cast<CGameEffect*>(s))
{}

effect::effect(effect &&other)
    : structure(nullptr)
{
    structure = other.structure;
    other.structure = nullptr;
}

effect::effect(const effect &other)
{
    if(!inited) {
        init_ptrs();
    }

    structure = (CGameEffect *)malloc(sizeof(CGameEffect));
    CGameEffect__ctor(structure, 0);
    CGameEffect__CopyEffect(structure, other.structure, 0);
}

effect &effect::operator=(const effect &rhs)
{
    if(this != &rhs) {
        if(!inited) {
            init_ptrs();
        }

        if(structure) {
            CGameEffect__dtor(structure, 1);
        }

        structure = (CGameEffect *)malloc(sizeof(CGameEffect));
        CGameEffect__ctor(structure, 0);
        CGameEffect__CopyEffect(structure, rhs.structure, 0);
    }

    return *this;
}

effect &effect::operator=(effect && rhs)
{
    if(this != &rhs) {
        if(structure) {
            CGameEffect__dtor(structure, 1);
        }

        structure = rhs.structure;
        rhs.structure = nullptr;
    }

    return *this;
}

effect::~effect()
{
    if(!inited) {
        init_ptrs();
    }

    if(structure)
        CGameEffect__dtor(structure, 1);
}

talent::talent(void *s)
    : structure(reinterpret_cast<CScriptTalent*>(s))
{}

talent::talent(talent &&other)
{
    if(structure) {
        free(structure);
    }

    structure = other.structure;
    other.structure = nullptr;
}

talent::talent(const talent &other)
{
    structure = (CScriptTalent*)malloc(sizeof(CScriptTalent));
    memcpy(structure, other.structure, sizeof(CScriptTalent));
}

talent &talent::operator=(const talent &rhs)
{
    if(this != &rhs) {
        if(structure) {
            free(structure);
        }

        structure = (CScriptTalent*)malloc(sizeof(CScriptTalent));
        memcpy(structure, rhs.structure, sizeof(CScriptTalent));
    }

    return *this;
}

talent &talent::operator=(talent && rhs)
{
    if(this != &rhs) {
        if(structure) {
            free(structure);
        }

        structure = rhs.structure;
        rhs.structure = nullptr;
    }

    return *this;
}

talent::~talent()
{
    if(structure) {
        free(structure);
    }
}

itemproperty::itemproperty()
{
    if(!inited) {
        init_ptrs();
    }

    structure = (CGameEffect *)malloc(sizeof(CGameEffect));
    CGameEffect__ctor(structure, 0);
    //TODO: check this...
}

itemproperty::itemproperty(void *s)
    : structure(reinterpret_cast<CGameEffect*>(s))
{}

itemproperty::itemproperty(itemproperty &&other)
    : structure(nullptr)
{
    structure = other.structure;
    other.structure = nullptr;
}

itemproperty::itemproperty(const itemproperty &other)
{
    if(!inited) {
        init_ptrs();
    }

    structure = (CGameEffect *)malloc(sizeof(CGameEffect));
    CGameEffect__ctor(structure, 0);
    CGameEffect__CopyEffect(structure, other.structure, 0);
}

itemproperty &itemproperty::operator=(const itemproperty &rhs)
{
    if(this != &rhs) {
        if(!inited) {
            init_ptrs();
        }

        if(structure) {
            CGameEffect__dtor(structure, 1);
        }

        structure = (CGameEffect *)malloc(sizeof(CGameEffect));
        CGameEffect__ctor(structure, 0);
        CGameEffect__CopyEffect(structure, rhs.structure, 0);
    }

    return *this;
}

itemproperty &itemproperty::operator=(itemproperty && rhs)
{
    if(this != &rhs) {
        if(structure) {
            CGameEffect__dtor(structure, 1);
        }

        structure = rhs.structure;
        rhs.structure = nullptr;
    }

    return *this;
}

itemproperty::~itemproperty()
{
    if(structure)
        CGameEffect__dtor(structure, 1);
}


event::event(void *s)
    : structure(reinterpret_cast<CScriptEvent *>(s))
{}

event::event(event &&other)
{
    if(structure) {
        CScriptEvent__dtor(structure, 1);
    }

    structure = other.structure;
    other.structure = nullptr;
}

event::event(const event &other)
{
    structure = (CScriptEvent*)malloc(sizeof(CScriptEvent));
    CScriptEvent__ctor(structure);
    CScriptEvent__CopyScriptEvent(structure, other.structure);
}

event &event::operator=(const event &rhs)
{
    if(this != &rhs) {
        if(structure) {
            CScriptEvent__dtor(structure, 1);
        }

        structure = (CScriptEvent*)malloc(sizeof(CScriptEvent));
        CScriptEvent__ctor(structure);
        CScriptEvent__CopyScriptEvent(structure, rhs.structure);
    }

    return *this;
}

event &event::operator=(event && rhs)
{
    if(this != &rhs) {
        if(structure) {
            CScriptEvent__dtor(structure, 1);
        }

        structure = rhs.structure;
        rhs.structure = nullptr;
    }

    return *this;
}

event::~event()
{
    if(structure)
        CScriptEvent__dtor(structure, 1);
}

location::location()
    : position { 0.0f, 0.0f, 0.0f }, orientation { 0.0f, 0.0f, 0.0f }, area {OBJECT_INVALID}
{}

location::location(vec3 position_, vec3 orientation_, object area_)
    : position(position_), orientation(orientation_), area(area_)
{}
