CFLAGS = -Wall -g
HOTRELOAD ?= true

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LIBEXT = dylib
	LIB_CFLAGS = -dynamiclib
else
	LIBEXT = so
	LIB_CFLAGS = -shared
endif


ifeq ($(HOTRELOAD),true)
CFLAGS += -DHOTRELOAD
LIBPLUG = libplug.$(LIBEXT)

all: main $(LIBPLUG)

main: main.c hotreload.c plug.c hotreload.h plug.h
	$(CC) $(CFLAGS) -o $@ main.c hotreload.c -ldl


libplug.$(LIBEXT): plug.c
	$(CC) $(CFLAGS) -fPIC $(LIB_CFLAGS) -o $@ plug.c
else
all: main
main: main.c plug.c hotreload.h plug.h
	$(CC) $(CFLAGS) -o $@ main.c plug.c
endif

clean:
	rm -f libplug.$(LIBEXT) main *.o
	rm -rf *.dSYM
