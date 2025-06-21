#ifndef HOTRELOAD_H
#define HOTRELOAD_H

#include <stdbool.h>
#include "plug.h"

#ifdef HOTRELOAD
    #define PLUG_INIT(State) do { \
        if (!reload_libplug()) return 1; \
        plug_init(NULL); \
    } while (0)
#else
    #define PLUG_INIT(State) do { \
        static State state = {0}; \
        if (!reload_libplug()) return 1; \
        plug_init(&state); \
    } while (0)
#endif

#ifdef HOTRELOAD
#define PLUG(name, ...) extern name##_t *name;
PLUG_LIST
#undef PLUG
bool reload_libplug(void);
#else
#define PLUG(name, ...) name##_t name;
PLUG_LIST
#undef PLUG
#define reload_libplug() true
#endif

#endif
