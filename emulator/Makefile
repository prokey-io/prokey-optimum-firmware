EMULATOR := 1

OBJS += setup.o

OBJS += buttons.o
OBJS += memory.o
OBJS += oled.o
OBJS += rng.o
OBJS += timer.o
OBJS += ws.o

OBJS += strl.o

libemulator.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

include ../Makefile.include
