#include <stdio.h>
#include <stdlib.h>
#include "hotreload.h"

int main() {
    if (!reload_libplug()) return 1;
    plug_init();

    while (1) {
        plug_update();
        char c = getchar();
        if (c == 'r') {
            void* state = plug_pre_reload();
            if (!reload_libplug()) return 1;
            plug_post_reload(state);
        }
    }

    return 0;
}
