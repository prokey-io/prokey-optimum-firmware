/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9.3 */

#include "messages-debug.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t DebugLinkDecision_fields[4] = {
    PB_FIELD(  1, BOOL    , OPTIONAL, STATIC  , FIRST, DebugLinkDecision, yes_no, yes_no, 0),
    PB_FIELD(  2, BOOL    , OPTIONAL, STATIC  , OTHER, DebugLinkDecision, up_down, yes_no, 0),
    PB_FIELD(  3, STRING  , OPTIONAL, STATIC  , OTHER, DebugLinkDecision, input, up_down, 0),
    PB_LAST_FIELD
};

const pb_field_t DebugLinkGetState_fields[1] = {
    PB_LAST_FIELD
};

const pb_field_t DebugLinkState_fields[13] = {
    PB_FIELD(  1, BYTES   , OPTIONAL, STATIC  , FIRST, DebugLinkState, layout, layout, 0),
    PB_FIELD(  2, STRING  , OPTIONAL, STATIC  , OTHER, DebugLinkState, pin, layout, 0),
    PB_FIELD(  3, STRING  , OPTIONAL, STATIC  , OTHER, DebugLinkState, matrix, pin, 0),
    PB_FIELD(  4, BYTES   , OPTIONAL, STATIC  , OTHER, DebugLinkState, mnemonic_secret, matrix, 0),
    PB_FIELD(  5, MESSAGE , OPTIONAL, STATIC  , OTHER, DebugLinkState, node, mnemonic_secret, &HDNodeType_fields),
    PB_FIELD(  6, BOOL    , OPTIONAL, STATIC  , OTHER, DebugLinkState, passphrase_protection, node, 0),
    PB_FIELD(  7, STRING  , OPTIONAL, STATIC  , OTHER, DebugLinkState, reset_word, passphrase_protection, 0),
    PB_FIELD(  8, BYTES   , OPTIONAL, STATIC  , OTHER, DebugLinkState, reset_entropy, reset_word, 0),
    PB_FIELD(  9, STRING  , OPTIONAL, STATIC  , OTHER, DebugLinkState, recovery_fake_word, reset_entropy, 0),
    PB_FIELD( 10, UINT32  , OPTIONAL, STATIC  , OTHER, DebugLinkState, recovery_word_pos, recovery_fake_word, 0),
    PB_FIELD( 11, UINT32  , OPTIONAL, STATIC  , OTHER, DebugLinkState, reset_word_pos, recovery_word_pos, 0),
    PB_FIELD( 12, UINT32  , OPTIONAL, STATIC  , OTHER, DebugLinkState, mnemonic_type, reset_word_pos, 0),
    PB_LAST_FIELD
};

const pb_field_t DebugLinkStop_fields[1] = {
    PB_LAST_FIELD
};

const pb_field_t DebugLinkLog_fields[4] = {
    PB_FIELD(  1, UINT32  , OPTIONAL, STATIC  , FIRST, DebugLinkLog, level, level, 0),
    PB_FIELD(  2, STRING  , OPTIONAL, STATIC  , OTHER, DebugLinkLog, bucket, level, 0),
    PB_FIELD(  3, STRING  , OPTIONAL, STATIC  , OTHER, DebugLinkLog, text, bucket, 0),
    PB_LAST_FIELD
};

const pb_field_t DebugLinkMemoryRead_fields[3] = {
    PB_FIELD(  1, UINT32  , OPTIONAL, STATIC  , FIRST, DebugLinkMemoryRead, address, address, 0),
    PB_FIELD(  2, UINT32  , OPTIONAL, STATIC  , OTHER, DebugLinkMemoryRead, length, address, 0),
    PB_LAST_FIELD
};

const pb_field_t DebugLinkMemory_fields[2] = {
    PB_FIELD(  1, BYTES   , OPTIONAL, STATIC  , FIRST, DebugLinkMemory, memory, memory, 0),
    PB_LAST_FIELD
};

const pb_field_t DebugLinkMemoryWrite_fields[4] = {
    PB_FIELD(  1, UINT32  , OPTIONAL, STATIC  , FIRST, DebugLinkMemoryWrite, address, address, 0),
    PB_FIELD(  2, BYTES   , OPTIONAL, STATIC  , OTHER, DebugLinkMemoryWrite, memory, address, 0),
    PB_FIELD(  3, BOOL    , OPTIONAL, STATIC  , OTHER, DebugLinkMemoryWrite, flash, memory, 0),
    PB_LAST_FIELD
};

const pb_field_t DebugLinkFlashErase_fields[2] = {
    PB_FIELD(  1, UINT32  , OPTIONAL, STATIC  , FIRST, DebugLinkFlashErase, sector, sector, 0),
    PB_LAST_FIELD
};


/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_32BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in 8 or 16 bit
 * field descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(DebugLinkState, layout) < 65536 && pb_membersize(DebugLinkState, node) < 65536 && pb_membersize(DebugLinkMemory, memory) < 65536 && pb_membersize(DebugLinkMemoryWrite, memory) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_DebugLinkDecision_DebugLinkGetState_DebugLinkState_DebugLinkStop_DebugLinkLog_DebugLinkMemoryRead_DebugLinkMemory_DebugLinkMemoryWrite_DebugLinkFlashErase)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
#error Field descriptor for DebugLinkMemoryWrite.memory is too large. Define PB_FIELD_16BIT to fix this.
#endif


/* @@protoc_insertion_point(eof) */
