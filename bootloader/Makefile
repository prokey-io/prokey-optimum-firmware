NAME  = bootloader

OBJS += bootloader.o
OBJS += usb.o
OBJS += updateAuthKey.o

OBJS += ../crypto/bignum.small.o
OBJS += ../crypto/ecdsa.small.o
OBJS += ../crypto/secp256k1.small.o
OBJS += ../crypto/sha2.small.o
OBJS += ../crypto/memzero.small.o
OBJS += ../crypto/aes/aescrypt.o
OBJS += ../crypto/aes/aeskey.o
OBJS += ../crypto/aes/aes_modes.o
OBJS += ../crypto/aes/aestab.o
OBJS += signatures.o

CFLAGS += -DUSE_PRECOMPUTED_IV=0
CFLAGS += -DUSE_PRECOMPUTED_CP=0

OPTFLAGS ?= -Os

include ../Makefile.include

align: $(NAME).bin
	./firmware_align.py $(NAME).bin

wtf: $(NAME).bin
	./flash.ubuntu.sh
