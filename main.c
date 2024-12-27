#include <stdio.h>
#include "hotreload.h"

int main() {
    INIT(State);

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
