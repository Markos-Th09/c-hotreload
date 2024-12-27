#include <stdio.h>
#include "hotreload.h"

#if defined(_WIN32)
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

#if defined(_WIN32)
    static const char *libplug_file_name = "libplug.dll";
#elif defined(__APPLE__)
    static const char *libplug_file_name = "libplug.dylib";
#else
    static const char *libplug_file_name = "libplug.so";
#endif

static void *libplug = NULL;

#define PLUG(name, ...) name##_t *name = NULL;
PLUG_LIST
#undef PLUG

static void* dl_open(const char* file_name)
{
    #ifdef _WIN32
        return LoadLibraryA(file_name);
    #else
        return dlopen(file_name, RTLD_NOW);
    #endif
}

static void* dl_sym(void* handle, const char* symbol)
{
    #ifdef _WIN32
        return GetProcAddress(handle, symbol);
    #else
        return dlsym(handle, symbol);
    #endif
}

static void dl_close(void* handle)
{
    #ifdef _WIN32
        FreeLibrary(handle);
    #else
        dlclose(handle);
    #endif
}

static char* dl_error()
{
    #ifdef _WIN32
        static char buffer[1024];
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, buffer, sizeof(buffer), NULL);
        return buffer;
    #else
        return dlerror();
    #endif
}

bool reload_libplug(void)
{
    if (libplug != NULL) dl_close(libplug);

    libplug = dl_open(libplug_file_name);
    if (libplug == NULL) {
        fprintf(stderr, "HOTRELOAD: could not load %s: %s", libplug_file_name, dl_error());
        return false;
    }

    #define PLUG(name, ...) \
        name = dl_sym(libplug, #name); \
        if (name == NULL) { \
            fprintf(stderr, "HOTRELOAD: could not find %s symbol in %s: %s", \
                     #name, libplug_file_name, dl_error()); \
            return false; \
        }
    PLUG_LIST
    #undef PLUG

    return true;
}
