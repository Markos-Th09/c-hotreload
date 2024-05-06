#ifndef HOTRELOAD_H
#define HOTRELOAD_H

#include <stdbool.h>
#include "plug.h"

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
