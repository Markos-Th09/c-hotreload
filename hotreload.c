#include <stdio.h>
#include <dlfcn.h>
#include "hotreload.h"

#ifdef __APPLE__
    static const char *libplug_file_name = "libplug.dylib";
#else
    static const char *libplug_file_name = "libplug.so";
#endif

static void *libplug = NULL;

#define PLUG(name, ...) name##_t *name = NULL;
PLUG_LIST
#undef PLUG

bool reload_libplug(void)
{
    if (libplug != NULL) dlclose(libplug);

    libplug = dlopen(libplug_file_name, RTLD_NOW);
    if (libplug == NULL) {
        fprintf(stderr, "HOTRELOAD: could not load %s: %s", libplug_file_name, dlerror());
        return false;
    }

    #define PLUG(name, ...) \
        name = dlsym(libplug, #name); \
        if (name == NULL) { \
            fprintf(stderr, "HOTRELOAD: could not find %s symbol in %s: %s", \
                     #name, libplug_file_name, dlerror()); \
            return false; \
        }
    PLUG_LIST
    #undef PLUG

    return true;
}
