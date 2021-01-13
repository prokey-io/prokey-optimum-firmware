// This file is automatically generated from coin_info.c.mako
// DO NOT EDIT

#include "coins.h"

#include "curves.h"
#include "secp256k1.h"

const CoinInfo coins[COINS_COUNT] = {
{
	.coin_name = "Bitcoin",
	.coin_shortcut = " BTC",
	.maxfee_kb = 2000000,
	.signed_message_header = "\x18" "Bitcoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 0,
	.address_type_p2sh = 5,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x049d7cb2,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "bc",
	.cashaddr_prefix = NULL,
	.coin_type = (0 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Testnet",
	.coin_shortcut = " TEST",
	.maxfee_kb = 10000000,
	.signed_message_header = "\x18" "Bitcoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 111,
	.address_type_p2sh = 196,
	.xpub_magic = 0x043587cf,
	.xpub_magic_segwit_p2sh = 0x044a5262,
	.xpub_magic_segwit_native = 0x045f1cf6,
	.fork_id = 0,
	.bech32_prefix = "tb",
	.cashaddr_prefix = NULL,
	.coin_type = (1 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Actinium",
	.coin_shortcut = " ACM",
	.maxfee_kb = 40000000,
	.signed_message_header = "\x19" "Actinium Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 53,
	.address_type_p2sh = 55,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x049d7cb2,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "acm",
	.cashaddr_prefix = NULL,
	.coin_type = (228 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Axe",
	.coin_shortcut = " AXE",
	.maxfee_kb = 100000,
	.signed_message_header = "\x19" "DarkCoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 55,
	.address_type_p2sh = 16,
	.xpub_magic = 0x02fe52cc,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (4242 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "BitCash",
	.coin_shortcut = " BITC",
	.maxfee_kb = 30000000,
	.signed_message_header = "\x18" "Bitcash Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 230,
	.address_type_p2sh = 235,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (230 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Bitcloud",
	.coin_shortcut = " BTDX",
	.maxfee_kb = 1000000,
	.signed_message_header = "\x18" "Diamond Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 25,
	.address_type_p2sh = 5,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (218 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Bcash",
	.coin_shortcut = " BCH",
	.maxfee_kb = 500000,
	.signed_message_header = "\x18" "Bitcoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = true,
	.force_bip143 = true,
	.decred = false,
	.address_type = 0,
	.address_type_p2sh = 5,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = "bitcoincash",
	.coin_type = (145 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Bcash Testnet",
	.coin_shortcut = " TBCH",
	.maxfee_kb = 10000000,
	.signed_message_header = "\x18" "Bitcoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = true,
	.force_bip143 = true,
	.decred = false,
	.address_type = 111,
	.address_type_p2sh = 196,
	.xpub_magic = 0x043587cf,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = "bchtest",
	.coin_type = (1 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Bgold",
	.coin_shortcut = " BTG",
	.maxfee_kb = 500000,
	.signed_message_header = "\x1d" "Bitcoin Gold Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = true,
	.force_bip143 = true,
	.decred = false,
	.address_type = 38,
	.address_type_p2sh = 23,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x049d7cb2,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 79,
	.bech32_prefix = "btg",
	.cashaddr_prefix = NULL,
	.coin_type = (156 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Bgold Testnet",
	.coin_shortcut = " TBTG",
	.maxfee_kb = 500000,
	.signed_message_header = "\x1d" "Bitcoin Gold Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = true,
	.force_bip143 = true,
	.decred = false,
	.address_type = 111,
	.address_type_p2sh = 196,
	.xpub_magic = 0x043587cf,
	.xpub_magic_segwit_p2sh = 0x044a5262,
	.xpub_magic_segwit_native = 0x045f1cf6,
	.fork_id = 79,
	.bech32_prefix = "tbtg",
	.cashaddr_prefix = NULL,
	.coin_type = (156 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Bprivate",
	.coin_shortcut = " BTCP",
	.maxfee_kb = 1000000,
	.signed_message_header = "\x1f" "BitcoinPrivate Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = true,
	.force_bip143 = false,
	.decred = false,
	.address_type = 4901,
	.address_type_p2sh = 5039,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 42,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (183 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Bitcore",
	.coin_shortcut = " BTX",
	.maxfee_kb = 2000000,
	.signed_message_header = "\x18" "BitCore Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 3,
	.address_type_p2sh = 125,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x049d7cb2,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "btx",
	.cashaddr_prefix = NULL,
	.coin_type = (160 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Bitsend",
	.coin_shortcut = " BSD",
	.maxfee_kb = 1000000,
	.signed_message_header = "\x18" "Bitsend Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 102,
	.address_type_p2sh = 5,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x049d7cb2,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "bsd",
	.cashaddr_prefix = NULL,
	.coin_type = (91 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Dash",
	.coin_shortcut = " DASH",
	.maxfee_kb = 100000,
	.signed_message_header = "\x19" "DarkCoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 76,
	.address_type_p2sh = 16,
	.xpub_magic = 0x02fe52cc,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (5 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Dash Testnet",
	.coin_shortcut = " tDASH",
	.maxfee_kb = 100000,
	.signed_message_header = "\x19" "DarkCoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 140,
	.address_type_p2sh = 19,
	.xpub_magic = 0x043587cf,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (1 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Decred",
	.coin_shortcut = " DCR",
	.maxfee_kb = 1000000,
	.signed_message_header = "\x17" "Decred Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = true,
	.address_type = 1855,
	.address_type_p2sh = 1818,
	.xpub_magic = 0x02fda926,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (42 | 0x80000000),
	.curve_name = SECP256K1_DECRED_NAME,
	.curve = &secp256k1_decred_info,
},
{
	.coin_name = "Decred Testnet",
	.coin_shortcut = " TDCR",
	.maxfee_kb = 10000000,
	.signed_message_header = "\x17" "Decred Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = true,
	.address_type = 3873,
	.address_type_p2sh = 3836,
	.xpub_magic = 0x043587d1,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (1 | 0x80000000),
	.curve_name = SECP256K1_DECRED_NAME,
	.curve = &secp256k1_decred_info,
},
{
	.coin_name = "Denarius",
	.coin_shortcut = " DNR",
	.maxfee_kb = 100000,
	.signed_message_header = "\x19" "Denarius Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 30,
	.address_type_p2sh = 90,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (116 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "DigiByte",
	.coin_shortcut = " DGB",
	.maxfee_kb = 500000,
	.signed_message_header = "\x19" "DigiByte Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 30,
	.address_type_p2sh = 63,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x049d7cb2,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "dgb",
	.cashaddr_prefix = NULL,
	.coin_type = (20 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Dogecoin",
	.coin_shortcut = " DOGE",
	.maxfee_kb = 1000000000,
	.signed_message_header = "\x19" "Dogecoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 30,
	.address_type_p2sh = 22,
	.xpub_magic = 0x02facafd,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (3 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Feathercoin",
	.coin_shortcut = " FTC",
	.maxfee_kb = 40000000,
	.signed_message_header = "\x1c" "Feathercoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 14,
	.address_type_p2sh = 5,
	.xpub_magic = 0x0488bc26,
	.xpub_magic_segwit_p2sh = 0x049d7cb2,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "fc",
	.cashaddr_prefix = NULL,
	.coin_type = (8 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Flashcoin",
	.coin_shortcut = " FLASH",
	.maxfee_kb = 4000000,
	.signed_message_header = "\x1a" "Flashcoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 68,
	.address_type_p2sh = 130,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x049d7cb2,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "flash",
	.cashaddr_prefix = NULL,
	.coin_type = (120 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Florincoin",
	.coin_shortcut = " FLO",
	.maxfee_kb = 40000000,
	.signed_message_header = "\x1b" "Florincoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 35,
	.address_type_p2sh = 94,
	.xpub_magic = 0x00174921,
	.xpub_magic_segwit_p2sh = 0x01b26ef6,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "flo",
	.cashaddr_prefix = NULL,
	.coin_type = (216 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Fujicoin",
	.coin_shortcut = " FJC",
	.maxfee_kb = 1000000000,
	.signed_message_header = "\x19" "FujiCoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 36,
	.address_type_p2sh = 16,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x049d7cb2,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "fc",
	.cashaddr_prefix = NULL,
	.coin_type = (75 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Gincoin",
	.coin_shortcut = " GIN",
	.maxfee_kb = 100000,
	.signed_message_header = "\x19" "DarkCoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 38,
	.address_type_p2sh = 10,
	.xpub_magic = 0x02fe52cc,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (2000 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "GameCredits",
	.coin_shortcut = " GAME",
	.maxfee_kb = 5000000,
	.signed_message_header = "\x1c" "GameCredits Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 38,
	.address_type_p2sh = 62,
	.xpub_magic = 0x019d9cfe,
	.xpub_magic_segwit_p2sh = 0x01b26ef6,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "game",
	.cashaddr_prefix = NULL,
	.coin_type = (101 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Groestlcoin",
	.coin_shortcut = " GRS",
	.maxfee_kb = 100000,
	.signed_message_header = "\x1c" "GroestlCoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 36,
	.address_type_p2sh = 5,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x049d7cb2,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "grs",
	.cashaddr_prefix = NULL,
	.coin_type = (17 | 0x80000000),
	.curve_name = SECP256K1_GROESTL_NAME,
	.curve = &secp256k1_groestl_info,
},
{
	.coin_name = "Groestlcoin Testnet",
	.coin_shortcut = " tGRS",
	.maxfee_kb = 100000,
	.signed_message_header = "\x1c" "GroestlCoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 111,
	.address_type_p2sh = 196,
	.xpub_magic = 0x043587cf,
	.xpub_magic_segwit_p2sh = 0x044a5262,
	.xpub_magic_segwit_native = 0x045f1cf6,
	.fork_id = 0,
	.bech32_prefix = "tgrs",
	.cashaddr_prefix = NULL,
	.coin_type = (1 | 0x80000000),
	.curve_name = SECP256K1_GROESTL_NAME,
	.curve = &secp256k1_groestl_info,
},
{
	.coin_name = "Komodo",
	.coin_shortcut = " KMD",
	.maxfee_kb = 1000000,
	.signed_message_header = "\x17" "Komodo Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 60,
	.address_type_p2sh = 85,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (141 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Koto",
	.coin_shortcut = " KOTO",
	.maxfee_kb = 1000000,
	.signed_message_header = "\x15" "Koto Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 6198,
	.address_type_p2sh = 6203,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (510 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Litecoin",
	.coin_shortcut = " LTC",
	.maxfee_kb = 40000000,
	.signed_message_header = "\x19" "Litecoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 48,
	.address_type_p2sh = 50,
	.xpub_magic = 0x019da462,
	.xpub_magic_segwit_p2sh = 0x01b26ef6,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "ltc",
	.cashaddr_prefix = NULL,
	.coin_type = (2 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Litecoin Testnet",
	.coin_shortcut = " tLTC",
	.maxfee_kb = 40000000,
	.signed_message_header = "\x19" "Litecoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 111,
	.address_type_p2sh = 58,
	.xpub_magic = 0x043587cf,
	.xpub_magic_segwit_p2sh = 0x044a5262,
	.xpub_magic_segwit_native = 0x045f1cf6,
	.fork_id = 0,
	.bech32_prefix = "tltc",
	.cashaddr_prefix = NULL,
	.coin_type = (1 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Megacoin",
	.coin_shortcut = " MEC",
	.maxfee_kb = 1000000,
	.signed_message_header = "\x19" "MegaCoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 50,
	.address_type_p2sh = 5,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x049d7cb2,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "mec",
	.cashaddr_prefix = NULL,
	.coin_type = (217 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Monacoin",
	.coin_shortcut = " MONA",
	.maxfee_kb = 5000000,
	.signed_message_header = "\x19" "Monacoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 50,
	.address_type_p2sh = 55,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x049d7cb2,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "mona",
	.cashaddr_prefix = NULL,
	.coin_type = (22 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "MonetaryUnit",
	.coin_shortcut = " MUE",
	.maxfee_kb = 100000,
	.signed_message_header = "\x1d" "MonetaryUnit Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 16,
	.address_type_p2sh = 76,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (31 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Myriad",
	.coin_shortcut = " XMY",
	.maxfee_kb = 2000000,
	.signed_message_header = "\x1b" "Myriadcoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 50,
	.address_type_p2sh = 9,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x049d7cb2,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "xmy",
	.cashaddr_prefix = NULL,
	.coin_type = (90 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "NIX",
	.coin_shortcut = " NIX",
	.maxfee_kb = 40000000,
	.signed_message_header = "\x14" "NIX Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 38,
	.address_type_p2sh = 53,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x049d7cb2,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "nix",
	.cashaddr_prefix = NULL,
	.coin_type = (400 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Namecoin",
	.coin_shortcut = " NMC",
	.maxfee_kb = 10000000,
	.signed_message_header = "\x19" "Namecoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 52,
	.address_type_p2sh = 5,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (7 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "PIVX",
	.coin_shortcut = " PIVX",
	.maxfee_kb = 100000,
	.signed_message_header = "\x18" "DarkNet Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 30,
	.address_type_p2sh = 13,
	.xpub_magic = 0x022d2533,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (119 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "PIVX Testnet",
	.coin_shortcut = " tPIVX",
	.maxfee_kb = 100000,
	.signed_message_header = "\x18" "DarkNet Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 139,
	.address_type_p2sh = 19,
	.xpub_magic = 0x3a8061a0,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (1 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Pesetacoin",
	.coin_shortcut = " PTC",
	.maxfee_kb = 1000000000,
	.signed_message_header = "\x1b" "Pesetacoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 47,
	.address_type_p2sh = 22,
	.xpub_magic = 0x0488c42e,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (109 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Primecoin",
	.coin_shortcut = " XPM",
	.maxfee_kb = 1000000,
	.signed_message_header = "\x1a" "Primecoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 23,
	.address_type_p2sh = 83,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (24 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Qtum",
	.coin_shortcut = " QTUM",
	.maxfee_kb = 40000000,
	.signed_message_header = "\x15" "Qtum Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 58,
	.address_type_p2sh = 50,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x049d7cb2,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "qc",
	.cashaddr_prefix = NULL,
	.coin_type = (2301 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Qtum Testnet",
	.coin_shortcut = " tQTUM",
	.maxfee_kb = 40000000,
	.signed_message_header = "\x15" "Qtum Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 120,
	.address_type_p2sh = 110,
	.xpub_magic = 0x043587cf,
	.xpub_magic_segwit_p2sh = 0x044a5262,
	.xpub_magic_segwit_native = 0x045f1cf6,
	.fork_id = 0,
	.bech32_prefix = "tq",
	.cashaddr_prefix = NULL,
	.coin_type = (1 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Ravencoin",
	.coin_shortcut = " RVN",
	.maxfee_kb = 2000000,
	.signed_message_header = "\x1a" "Ravencoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 60,
	.address_type_p2sh = 122,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (175 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Regtest",
	.coin_shortcut = " REGTEST",
	.maxfee_kb = 10000000,
	.signed_message_header = "\x18" "Bitcoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 111,
	.address_type_p2sh = 196,
	.xpub_magic = 0x043587cf,
	.xpub_magic_segwit_p2sh = 0x044a5262,
	.xpub_magic_segwit_native = 0x045f1cf6,
	.fork_id = 0,
	.bech32_prefix = "bcrt",
	.cashaddr_prefix = NULL,
	.coin_type = (1 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "SmartCash",
	.coin_shortcut = " SMART",
	.maxfee_kb = 1000000,
	.signed_message_header = "\x1a" "SmartCash Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 63,
	.address_type_p2sh = 18,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (224 | 0x80000000),
	.curve_name = SECP256K1_SMART_NAME,
	.curve = &secp256k1_smart_info,
},
{
	.coin_name = "SmartCash Testnet",
	.coin_shortcut = " tSMART",
	.maxfee_kb = 1000000,
	.signed_message_header = "\x1a" "SmartCash Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 65,
	.address_type_p2sh = 21,
	.xpub_magic = 0x043587cf,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (224 | 0x80000000),
	.curve_name = SECP256K1_SMART_NAME,
	.curve = &secp256k1_smart_info,
},
{
	.coin_name = "Stakenet",
	.coin_shortcut = " XSN",
	.maxfee_kb = 2000000,
	.signed_message_header = "\x19" "DarkCoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 76,
	.address_type_p2sh = 16,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x049d7cb2,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "xc",
	.cashaddr_prefix = NULL,
	.coin_type = (199 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Vertcoin",
	.coin_shortcut = " VTC",
	.maxfee_kb = 40000000,
	.signed_message_header = "\x19" "Vertcoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 71,
	.address_type_p2sh = 5,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x049d7cb2,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "vtc",
	.cashaddr_prefix = NULL,
	.coin_type = (28 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Viacoin",
	.coin_shortcut = " VIA",
	.maxfee_kb = 40000000,
	.signed_message_header = "\x18" "Viacoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = true,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 71,
	.address_type_p2sh = 33,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x049d7cb2,
	.xpub_magic_segwit_native = 0x04b24746,
	.fork_id = 0,
	.bech32_prefix = "via",
	.cashaddr_prefix = NULL,
	.coin_type = (14 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "ZClassic",
	.coin_shortcut = " ZCL",
	.maxfee_kb = 1000000,
	.signed_message_header = "\x16" "Zcash Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 7352,
	.address_type_p2sh = 7357,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (147 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Zcash",
	.coin_shortcut = " ZEC",
	.maxfee_kb = 1000000,
	.signed_message_header = "\x16" "Zcash Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 7352,
	.address_type_p2sh = 7357,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (133 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Zcash Testnet",
	.coin_shortcut = " TAZ",
	.maxfee_kb = 10000000,
	.signed_message_header = "\x16" "Zcash Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 7461,
	.address_type_p2sh = 7354,
	.xpub_magic = 0x043587cf,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (1 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Zcoin",
	.coin_shortcut = " XZC",
	.maxfee_kb = 1000000,
	.signed_message_header = "\x16" "Zcoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 82,
	.address_type_p2sh = 7,
	.xpub_magic = 0x0488b21e,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (136 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
{
	.coin_name = "Zcoin Testnet",
	.coin_shortcut = " tXZC",
	.maxfee_kb = 1000000,
	.signed_message_header = "\x16" "Zcoin Signed Message:\n",
	.has_address_type = true,
	.has_address_type_p2sh = true,
	.has_segwit = false,
	.has_fork_id = false,
	.force_bip143 = false,
	.decred = false,
	.address_type = 65,
	.address_type_p2sh = 178,
	.xpub_magic = 0x043587cf,
	.xpub_magic_segwit_p2sh = 0x00000000,
	.xpub_magic_segwit_native = 0x00000000,
	.fork_id = 0,
	.bech32_prefix = NULL,
	.cashaddr_prefix = NULL,
	.coin_type = (1 | 0x80000000),
	.curve_name = SECP256K1_NAME,
	.curve = &secp256k1_info,
},
};
