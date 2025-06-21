#include <stddef.h>

#ifndef PLUG_H
#define PLUG_H

#define STATE_FIELDS \
    STATE(int, x) \
    STATE(int, y)

#ifdef HOTRELOAD
typedef struct  {
    const char* name;
    const char* type;
    size_t offset;
    size_t size;
} RttiEntry;
#endif

#define STATE(type, name, ...) type name __VA_ARGS__;
typedef struct {
    #ifdef HOTRELOAD
    RttiEntry* rtti;
    size_t rtti_len;
    size_t version;
    #endif
    STATE_FIELDS
} State;
#undef STATE

#define PLUG_LIST \
    PLUG(plug_init, void, void*) \
    PLUG(plug_pre_reload, void*, void) \
    PLUG(plug_post_reload, void, void*) \
    PLUG(plug_update, void, void)

#define PLUG(name, ret, ...) typedef ret (name##_t)(__VA_ARGS__);
PLUG_LIST
#undef PLUG
#endif
