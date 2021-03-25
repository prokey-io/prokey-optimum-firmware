TOP_DIR       := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
TOOLCHAIN_DIR ?= ../vendor/libopencm3

ifneq ($(V),1)
Q := @
endif

NAME  = firstboot

PREFIX   ?= arm-none-eabi-
CC       := $(PREFIX)gcc
LD       := $(PREFIX)gcc
OBJCOPY  := $(PREFIX)objcopy
OBJDUMP  := $(PREFIX)objdump
AR       := $(PREFIX)ar
AS       := $(PREFIX)as
LDSCRIPT  = memory.ld

OPTFLAGS ?= -O3
DBGFLAGS ?= -g -DNDEBUG
CPUFLAGS ?= -mcpu=cortex-m3 -mthumb
FPUFLAGS ?= -msoft-float

CFLAGS   += $(OPTFLAGS) \
            $(DBGFLAGS) \
            -std=gnu11 \
            -W \
            -Wall \
            -Wextra \
            -Wimplicit-function-declaration \
            -Wredundant-decls \
            -Wstrict-prototypes \
            -Wundef \
            -Wshadow \
            -Wpointer-arith \
            -Wformat \
            -Wreturn-type \
            -Wsign-compare \
            -Wmultichar \
            -Wformat-nonliteral \
            -Winit-self \
            -Wuninitialized \
            -Wformat-security \
            -fno-common \
            -fno-exceptions \
            -fvisibility=internal \
            -ffunction-sections \
            -fdata-sections \
            -fstack-protector-all \
            $(CPUFLAGS) \
            $(FPUFLAGS) \
            -DSTM32F2 \
            -DRAND_PLATFORM_INDEPENDENT=1 \
            -I$(TOOLCHAIN_DIR)/include \

LDFLAGS  += -L$(TOP_DIR) \
            $(DBGFLAGS) \
            $(CPUFLAGS) \
            $(FPUFLAGS) \
			--static \
            -Wl,--start-group \
            -lc \
            -lgcc \
            -lnosys \
            -Wl,--end-group \
            -L$(TOOLCHAIN_DIR)/lib \
            -T$(LDSCRIPT) \
            -nostartfiles \
            -Wl,--gc-sections

LDLIBS   += -lopencm3_stm32f2
LIBDEPS  += $(TOOLCHAIN_DIR)/lib/libopencm3_stm32f2.a

OBJS += firstboot.o

all: $(NAME).bin

wtf: $(NAME).bin
	./flash.ubuntu.sh

$(NAME).bin: $(NAME).elf
	@printf "  OBJCOPY $@\n"
	$(Q)$(OBJCOPY) -Obinary $(NAME).elf $(NAME).bin

$(NAME).hex: $(NAME).elf
	@printf "  OBJCOPY $@\n"
	$(Q)$(OBJCOPY) -Oihex $(NAME).elf $(NAME).hex

$(NAME).srec: $(NAME).elf
	@printf "  OBJCOPY $@\n"
	$(Q)$(OBJCOPY) -Osrec $(NAME).elf $(NAME).srec

$(NAME).list: $(NAME).elf
	@printf "  OBJDUMP $@\n"
	$(Q)$(OBJDUMP) -S $(NAME).elf > $(NAME).list

$(NAME).elf: $(OBJS) $(LDSCRIPT) $(LIBDEPS)
	@printf "  LD      $@\n"
	$(Q)$(LD) -o $(NAME).elf $(OBJS) $(LDLIBS) $(LDFLAGS)

%.o: %.s Makefile
	@printf "  AS      $@\n"
	$(Q)$(AS) $(CPUFLAGS) -o $@ $<

%.o: %.c Makefile
	@printf "  CC      $@\n"
	$(Q)$(CC) $(CFLAGS) -MMD -MP -o $@ -c $<

clean:
	rm -f *.bin *.d *.o *.hex *.elf *.list *.srec

-include $(OBJS:.o=.d)
