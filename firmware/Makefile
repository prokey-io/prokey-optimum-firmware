APPVER = 1.8.0

NAME  = prokey

ifeq ($(EMULATOR),1)
OBJS += udp.o
else
OBJS += usb.o
OBJS += bl_check.o
OBJS += otp.o
OBJS += ../auth.o
endif

OBJS += messages.o
OBJS += config.o
OBJS += main.o
OBJS += pinmatrix.o
OBJS += fsm.o
OBJS += coins.o
OBJS += coin_info.o
OBJS += transaction.o
OBJS += protect.o
OBJS += layout2.o
OBJS += recovery.o
OBJS += reset.o
OBJS += signing.o
OBJS += crypto.o
OBJS += prokey_layout.o
OBJS += prokey_recovery.o
OBJS += pin_number.o


ifneq ($(U2F_ENABLED),0)
OBJS += u2f.o
endif

ifneq ($(BITCOIN_ONLY),1)
OBJS += ethereum.o
OBJS += ethereum_tokens.o
OBJS += lisk.o
OBJS += nem2.o
OBJS += nem_mosaics.o
OBJS += stellar.o
endif

OBJS += debug.o

OBJS += ../crypto/address.o
OBJS += ../crypto/bignum.o
OBJS += ../crypto/ecdsa.o
OBJS += ../crypto/curves.o
OBJS += ../crypto/secp256k1.o
OBJS += ../crypto/nist256p1.o
OBJS += ../crypto/rand.o
OBJS += ../crypto/memzero.o

OBJS += ../crypto/ed25519-donna/curve25519-donna-32bit.o
OBJS += ../crypto/ed25519-donna/curve25519-donna-helpers.o
OBJS += ../crypto/ed25519-donna/modm-donna-32bit.o
OBJS += ../crypto/ed25519-donna/ed25519-donna-basepoint-table.o
OBJS += ../crypto/ed25519-donna/ed25519-donna-32bit-tables.o
OBJS += ../crypto/ed25519-donna/ed25519-donna-impl-base.o
OBJS += ../crypto/ed25519-donna/ed25519.o
OBJS += ../crypto/ed25519-donna/curve25519-donna-scalarmult-base.o
OBJS += ../crypto/ed25519-donna/ed25519-sha3.o
OBJS += ../crypto/ed25519-donna/ed25519-keccak.o

OBJS += ../crypto/hmac.o
OBJS += ../crypto/bip32.o
OBJS += ../crypto/bip39.o
OBJS += ../crypto/pbkdf2.o
OBJS += ../crypto/base32.o
OBJS += ../crypto/base58.o
OBJS += ../crypto/segwit_addr.o
OBJS += ../crypto/cash_addr.o

OBJS += ../crypto/ripemd160.o
OBJS += ../crypto/sha2.o
OBJS += ../crypto/sha3.o
OBJS += ../crypto/blake256.o
OBJS += ../crypto/blake2b.o
OBJS += ../crypto/groestl.o
OBJS += ../crypto/hasher.o

OBJS += ../crypto/aes/aescrypt.o
OBJS += ../crypto/aes/aeskey.o
OBJS += ../crypto/aes/aestab.o
OBJS += ../crypto/aes/aes_modes.o

OBJS += ../crypto/chacha20poly1305/chacha20poly1305.o
OBJS += ../crypto/chacha20poly1305/chacha_merged.o
OBJS += ../crypto/chacha20poly1305/poly1305-donna.o
OBJS += ../crypto/chacha20poly1305/rfc7539.o

OBJS += ../crypto/nem.o

OBJS += ../vendor/QR-Code-generator/c/qrcodegen.o

OBJS += ../storage/storage.o
OBJS += ../storage/norcow.o

OBJS += ../vendor/nanopb/pb_common.o
OBJS += ../vendor/nanopb/pb_decode.o
OBJS += ../vendor/nanopb/pb_encode.o

OBJS += protob/messages.pb.o
OBJS += protob/messages-bitcoin.pb.o
OBJS += protob/messages-common.pb.o
OBJS += protob/messages-crypto.pb.o
OBJS += protob/messages-debug.pb.o
OBJS += protob/messages-management.pb.o
ifneq ($(BITCOIN_ONLY),1)
OBJS += protob/messages-ethereum.pb.o
OBJS += protob/messages-lisk.pb.o
OBJS += protob/messages-nem.pb.o
OBJS += protob/messages-ripple.pb.o
OBJS += protob/messages-stellar.pb.o
OBJS += protob/messages-tron.pb.o
endif

OPTFLAGS ?= -Os

../crypto/bip32.o: OPTFLAGS = -O3
../crypto/bip39.o: OPTFLAGS = -O3
../crypto/ecdsa.o: OPTFLAGS = -O3
../crypto/sha2.o: OPTFLAGS = -O3
../crypto/secp256k1.o: OPTFLAGS = -O3

include ../Makefile.include

DEBUG_LINK ?= 0
DEBUG_LOG  ?= 0

CFLAGS += -Wno-sequence-point
CFLAGS += -I../vendor/nanopb -Iprotob -DPB_FIELD_16BIT=1
CFLAGS += -DDEBUG_LINK=$(DEBUG_LINK)
CFLAGS += -DDEBUG_LOG=$(DEBUG_LOG)
CFLAGS += -DSCM_REVISION='"$(shell git rev-parse HEAD | sed 's:\(..\):\\x\1:g')"'
CFLAGS += -DUSE_MONERO=0
ifneq ($(BITCOIN_ONLY),1)
CFLAGS += -DUSE_ETHEREUM=1
CFLAGS += -DUSE_NEM=1
MAKO_RENDER_FLAG =
else
CFLAGS += -DUSE_ETHEREUM=0
CFLAGS += -DUSE_NEM=0
MAKO_RENDER_FLAG = --bitcoin-only
endif

%:: %.mako defs
	@printf "  MAKO    $@\n"
	$(Q)$(PYTHON) ../common/tools/cointool.py render $(MAKO_RENDER_FLAG) $@.mako

bl_data.h: bl_data.py
	@printf "  PYTHON  bl_data.py\n"
	$(Q)$(PYTHON) bl_data.py

clean::
	rm -f bl_data.h
	find -maxdepth 1 -name "*.mako" | sed 's/.mako$$//' | xargs rm -f

wtf: $(NAME).bin
	./flash.sh

lwtf: $(NAME).bin
	./flash.ubuntu.sh
