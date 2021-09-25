#ifndef TALIB_SCRIPTS_TYPES_H
#define TALIB_SCRIPTS_TYPES_H

#include <string>
#include "nx_devel.h"
#include "nx_log.h"
#include "api/all.h"
#include "ScriptLib.h"

typedef nwobjid object;

struct effect {
    effect();
    effect(void *s);
    effect(effect&& other);
    effect(const effect& other);
    effect& operator=(const effect& rhs);
    effect& operator=(effect&& rhs);
    ~effect();

    CGameEffect *structure = nullptr;
};

struct talent {
    talent(void *s);
    talent(talent&& other);
    talent(const talent& other);
    talent& operator=(const talent& rhs);
    talent& operator=(talent&& rhs);
    ~talent();

    CScriptTalent *structure = nullptr;
};

struct event {
    event(void *s);
    event(event&& other);
    event(const event& other);
    event& operator=(const event& rhs);
    event& operator=(event &&rhs);
    ~event();

    CScriptEvent *structure = nullptr;
};

struct itemproperty {
    itemproperty();
    itemproperty(void *s);
    itemproperty(itemproperty&& other);
    itemproperty(const itemproperty& other);
    itemproperty& operator=(const itemproperty& rhs);
    itemproperty& operator=(itemproperty &&rhs);
    ~itemproperty();

    CGameEffect *structure = nullptr;
};

struct vec3 {
    vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    float x, y, z;
};

struct location {
    location();
    location(vec3 position_, vec3 orientation_, object area_);
    location(location&& other) = default;
    location(const location& other) = default;
    location& operator=(const location& rhs) = default;
    location& operator=(location&& rhs) = default;

    vec3 position;
    vec3 orientation;
    object area;
};


typedef std::string string;

#endif // TALIB_SCRIPTS_TYPES_H
