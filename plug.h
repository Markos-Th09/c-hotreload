#ifndef PLUG_H
#define PLUG_H

#define PLUG_LIST \
    PLUG(plug_init, void, void) \
    PLUG(plug_pre_reload, void*, void) \
    PLUG(plug_post_reload, void, void*) \
    PLUG(plug_update, void, void)

#define PLUG(name, ret, ...) typedef ret (name##_t)(__VA_ARGS__);
PLUG_LIST
#undef PLUG

#endif
