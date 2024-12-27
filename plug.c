#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "plug.h"

#ifdef HOTRELOAD
    #define STATE(type, name, ...) {#name, #type, offsetof(State, name), sizeof(((State*)0)->name)},
    static const RttiEntry RTTI_ENTRIES[] = {
        STATE_FIELDS
    };
    #undef STATE

    #define STATE(type, name, ...) +sizeof(#type)+sizeof(#name)
    static const size_t STATE_ARENA_SIZE = sizeof(State)+sizeof(RTTI_ENTRIES)+STATE_FIELDS;
    #undef STATE

    static inline size_t strlencpy(char* dest, const char* src) {
        size_t i = 0;
        while (src[i] != '\0') {
            dest[i] = src[i];
            i++;
        }
        dest[i] = '\0';
        return i;
    }
#endif

static State* state = NULL;

static inline void init_state(void* s);
static inline void merge_state(State* old_state) {
    #ifdef HOTRELOAD
        init_state(NULL);
        for (size_t i = 0; i < sizeof(RTTI_ENTRIES)/sizeof(RttiEntry); i++) {
            RttiEntry entry = RTTI_ENTRIES[i];
            for (size_t j = 0; j < old_state->rtti_len; j++) {
                RttiEntry old_entry = old_state->rtti[j];
                if (strcmp(old_entry.name, entry.name) == 0
                    && strcmp(old_entry.type, entry.type) == 0
                    && old_entry.size == entry.size) {
                    memcpy((char*)state+entry.offset, (char*)old_state+entry.offset, entry.size);
                    break;
                }
            }
        }
        free(old_state);
    #else
        state = old_state;
    #endif
}

static inline void alloc_state_arena(void* s) {
    #ifdef HOTRELOAD
        state = malloc(STATE_ARENA_SIZE);
        state->rtti = (RttiEntry*)(state+1);
        state->rtti_len = sizeof(RTTI_ENTRIES)/sizeof(RttiEntry);

        char* strp = (char*)state->rtti+sizeof(RTTI_ENTRIES);
        for (size_t i = 0; i < state->rtti_len; i++) {
            RttiEntry entry = RTTI_ENTRIES[i];
            state->rtti[i].size = entry.size;
            state->rtti[i].offset = entry.offset;

            state->rtti[i].name = strp;
            strp += strlencpy(strp, entry.name)+1;

            state->rtti[i].type = strp;
            strp += strlencpy(strp, entry.type)+1;
        }
    #else
        state = s;
    #endif
}

static inline void init_state(void* s) {
    alloc_state_arena(s);
    state->x = 1;
    state->y = 2;
}

void plug_init(void* plug) {
    init_state(plug);
}

State* plug_pre_reload(void){
    return state;
}

void plug_post_reload(void* state) {
    merge_state(state);
}

void plug_update(void) {
    printf("x: %d, y: %d\n", state->x++, state->y++);
}
