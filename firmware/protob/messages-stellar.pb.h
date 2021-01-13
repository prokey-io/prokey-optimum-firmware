/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9.3 */

#ifndef PB_HW_TREZOR_MESSAGES_STELLAR_MESSAGES_STELLAR_PB_H_INCLUDED
#define PB_HW_TREZOR_MESSAGES_STELLAR_MESSAGES_STELLAR_PB_H_INCLUDED
#include "pb.h"
/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _StellarTxOpRequest {
    char dummy_field;
/* @@protoc_insertion_point(struct:StellarTxOpRequest) */
} StellarTxOpRequest;

typedef struct _StellarAccountMergeOp {
    bool has_source_account;
    char source_account[57];
    bool has_destination_account;
    char destination_account[57];
/* @@protoc_insertion_point(struct:StellarAccountMergeOp) */
} StellarAccountMergeOp;

typedef struct _StellarAddress {
    bool has_address;
    char address[57];
/* @@protoc_insertion_point(struct:StellarAddress) */
} StellarAddress;

typedef struct _StellarAllowTrustOp {
    bool has_source_account;
    char source_account[57];
    bool has_trusted_account;
    char trusted_account[57];
    bool has_asset_type;
    uint32_t asset_type;
    bool has_asset_code;
    char asset_code[13];
    bool has_is_authorized;
    uint32_t is_authorized;
/* @@protoc_insertion_point(struct:StellarAllowTrustOp) */
} StellarAllowTrustOp;

typedef struct _StellarAssetType {
    bool has_type;
    uint32_t type;
    bool has_code;
    char code[13];
    bool has_issuer;
    char issuer[57];
/* @@protoc_insertion_point(struct:StellarAssetType) */
} StellarAssetType;

typedef struct _StellarBumpSequenceOp {
    bool has_source_account;
    char source_account[57];
    bool has_bump_to;
    uint64_t bump_to;
/* @@protoc_insertion_point(struct:StellarBumpSequenceOp) */
} StellarBumpSequenceOp;

typedef struct _StellarCreateAccountOp {
    bool has_source_account;
    char source_account[57];
    bool has_new_account;
    char new_account[57];
    bool has_starting_balance;
    int64_t starting_balance;
/* @@protoc_insertion_point(struct:StellarCreateAccountOp) */
} StellarCreateAccountOp;

typedef struct _StellarGetAddress {
    pb_size_t address_n_count;
    uint32_t address_n[10];
    bool has_show_display;
    bool show_display;
/* @@protoc_insertion_point(struct:StellarGetAddress) */
} StellarGetAddress;

typedef PB_BYTES_ARRAY_T(65) StellarManageDataOp_value_t;
typedef struct _StellarManageDataOp {
    bool has_source_account;
    char source_account[57];
    bool has_key;
    char key[65];
    bool has_value;
    StellarManageDataOp_value_t value;
/* @@protoc_insertion_point(struct:StellarManageDataOp) */
} StellarManageDataOp;

typedef PB_BYTES_ARRAY_T(32) StellarSetOptionsOp_signer_key_t;
typedef struct _StellarSetOptionsOp {
    bool has_source_account;
    char source_account[57];
    bool has_inflation_destination_account;
    char inflation_destination_account[57];
    bool has_clear_flags;
    uint32_t clear_flags;
    bool has_set_flags;
    uint32_t set_flags;
    bool has_master_weight;
    uint32_t master_weight;
    bool has_low_threshold;
    uint32_t low_threshold;
    bool has_medium_threshold;
    uint32_t medium_threshold;
    bool has_high_threshold;
    uint32_t high_threshold;
    bool has_home_domain;
    char home_domain[33];
    bool has_signer_type;
    uint32_t signer_type;
    bool has_signer_key;
    StellarSetOptionsOp_signer_key_t signer_key;
    bool has_signer_weight;
    uint32_t signer_weight;
/* @@protoc_insertion_point(struct:StellarSetOptionsOp) */
} StellarSetOptionsOp;

typedef PB_BYTES_ARRAY_T(32) StellarSignTx_memo_hash_t;
typedef struct _StellarSignTx {
    pb_size_t address_n_count;
    uint32_t address_n[10];
    bool has_network_passphrase;
    char network_passphrase[1024];
    bool has_source_account;
    char source_account[57];
    bool has_fee;
    uint32_t fee;
    bool has_sequence_number;
    uint64_t sequence_number;
    bool has_timebounds_start;
    uint32_t timebounds_start;
    bool has_timebounds_end;
    uint32_t timebounds_end;
    bool has_memo_type;
    uint32_t memo_type;
    bool has_memo_text;
    char memo_text[29];
    bool has_memo_id;
    uint64_t memo_id;
    bool has_memo_hash;
    StellarSignTx_memo_hash_t memo_hash;
    bool has_num_operations;
    uint32_t num_operations;
/* @@protoc_insertion_point(struct:StellarSignTx) */
} StellarSignTx;

typedef PB_BYTES_ARRAY_T(32) StellarSignedTx_public_key_t;
typedef PB_BYTES_ARRAY_T(64) StellarSignedTx_signature_t;
typedef struct _StellarSignedTx {
    bool has_public_key;
    StellarSignedTx_public_key_t public_key;
    bool has_signature;
    StellarSignedTx_signature_t signature;
/* @@protoc_insertion_point(struct:StellarSignedTx) */
} StellarSignedTx;

typedef struct _StellarChangeTrustOp {
    bool has_source_account;
    char source_account[57];
    bool has_asset;
    StellarAssetType asset;
    bool has_limit;
    uint64_t limit;
/* @@protoc_insertion_point(struct:StellarChangeTrustOp) */
} StellarChangeTrustOp;

typedef struct _StellarCreatePassiveOfferOp {
    bool has_source_account;
    char source_account[57];
    bool has_selling_asset;
    StellarAssetType selling_asset;
    bool has_buying_asset;
    StellarAssetType buying_asset;
    bool has_amount;
    int64_t amount;
    bool has_price_n;
    uint32_t price_n;
    bool has_price_d;
    uint32_t price_d;
/* @@protoc_insertion_point(struct:StellarCreatePassiveOfferOp) */
} StellarCreatePassiveOfferOp;

typedef struct _StellarManageOfferOp {
    bool has_source_account;
    char source_account[57];
    bool has_selling_asset;
    StellarAssetType selling_asset;
    bool has_buying_asset;
    StellarAssetType buying_asset;
    bool has_amount;
    int64_t amount;
    bool has_price_n;
    uint32_t price_n;
    bool has_price_d;
    uint32_t price_d;
    bool has_offer_id;
    uint64_t offer_id;
/* @@protoc_insertion_point(struct:StellarManageOfferOp) */
} StellarManageOfferOp;

typedef struct _StellarPathPaymentOp {
    bool has_source_account;
    char source_account[57];
    bool has_send_asset;
    StellarAssetType send_asset;
    bool has_send_max;
    int64_t send_max;
    bool has_destination_account;
    char destination_account[57];
    bool has_destination_asset;
    StellarAssetType destination_asset;
    bool has_destination_amount;
    int64_t destination_amount;
    pb_size_t paths_count;
    StellarAssetType paths[5];
/* @@protoc_insertion_point(struct:StellarPathPaymentOp) */
} StellarPathPaymentOp;

typedef struct _StellarPaymentOp {
    bool has_source_account;
    char source_account[57];
    bool has_destination_account;
    char destination_account[57];
    bool has_asset;
    StellarAssetType asset;
    bool has_amount;
    int64_t amount;
/* @@protoc_insertion_point(struct:StellarPaymentOp) */
} StellarPaymentOp;

/* Default values for struct fields */

/* Initializer values for message structs */
#define StellarAssetType_init_default            {false, 0, false, "", false, ""}
#define StellarGetAddress_init_default           {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, false, 0}
#define StellarAddress_init_default              {false, ""}
#define StellarSignTx_init_default               {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, false, "", false, "", false, 0, false, 0, false, 0, false, 0, false, 0, false, "", false, 0, false, {0, {0}}, false, 0}
#define StellarTxOpRequest_init_default          {0}
#define StellarPaymentOp_init_default            {false, "", false, "", false, StellarAssetType_init_default, false, 0}
#define StellarCreateAccountOp_init_default      {false, "", false, "", false, 0}
#define StellarPathPaymentOp_init_default        {false, "", false, StellarAssetType_init_default, false, 0, false, "", false, StellarAssetType_init_default, false, 0, 0, {StellarAssetType_init_default, StellarAssetType_init_default, StellarAssetType_init_default, StellarAssetType_init_default, StellarAssetType_init_default}}
#define StellarManageOfferOp_init_default        {false, "", false, StellarAssetType_init_default, false, StellarAssetType_init_default, false, 0, false, 0, false, 0, false, 0}
#define StellarCreatePassiveOfferOp_init_default {false, "", false, StellarAssetType_init_default, false, StellarAssetType_init_default, false, 0, false, 0, false, 0}
#define StellarSetOptionsOp_init_default         {false, "", false, "", false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, "", false, 0, false, {0, {0}}, false, 0}
#define StellarChangeTrustOp_init_default        {false, "", false, StellarAssetType_init_default, false, 0}
#define StellarAllowTrustOp_init_default         {false, "", false, "", false, 0, false, "", false, 0}
#define StellarAccountMergeOp_init_default       {false, "", false, ""}
#define StellarManageDataOp_init_default         {false, "", false, "", false, {0, {0}}}
#define StellarBumpSequenceOp_init_default       {false, "", false, 0}
#define StellarSignedTx_init_default             {false, {0, {0}}, false, {0, {0}}}
#define StellarAssetType_init_zero               {false, 0, false, "", false, ""}
#define StellarGetAddress_init_zero              {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, false, 0}
#define StellarAddress_init_zero                 {false, ""}
#define StellarSignTx_init_zero                  {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, false, "", false, "", false, 0, false, 0, false, 0, false, 0, false, 0, false, "", false, 0, false, {0, {0}}, false, 0}
#define StellarTxOpRequest_init_zero             {0}
#define StellarPaymentOp_init_zero               {false, "", false, "", false, StellarAssetType_init_zero, false, 0}
#define StellarCreateAccountOp_init_zero         {false, "", false, "", false, 0}
#define StellarPathPaymentOp_init_zero           {false, "", false, StellarAssetType_init_zero, false, 0, false, "", false, StellarAssetType_init_zero, false, 0, 0, {StellarAssetType_init_zero, StellarAssetType_init_zero, StellarAssetType_init_zero, StellarAssetType_init_zero, StellarAssetType_init_zero}}
#define StellarManageOfferOp_init_zero           {false, "", false, StellarAssetType_init_zero, false, StellarAssetType_init_zero, false, 0, false, 0, false, 0, false, 0}
#define StellarCreatePassiveOfferOp_init_zero    {false, "", false, StellarAssetType_init_zero, false, StellarAssetType_init_zero, false, 0, false, 0, false, 0}
#define StellarSetOptionsOp_init_zero            {false, "", false, "", false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, "", false, 0, false, {0, {0}}, false, 0}
#define StellarChangeTrustOp_init_zero           {false, "", false, StellarAssetType_init_zero, false, 0}
#define StellarAllowTrustOp_init_zero            {false, "", false, "", false, 0, false, "", false, 0}
#define StellarAccountMergeOp_init_zero          {false, "", false, ""}
#define StellarManageDataOp_init_zero            {false, "", false, "", false, {0, {0}}}
#define StellarBumpSequenceOp_init_zero          {false, "", false, 0}
#define StellarSignedTx_init_zero                {false, {0, {0}}, false, {0, {0}}}

/* Field tags (for use in manual encoding/decoding) */
#define StellarAccountMergeOp_source_account_tag 1
#define StellarAccountMergeOp_destination_account_tag 2
#define StellarAddress_address_tag               1
#define StellarAllowTrustOp_source_account_tag   1
#define StellarAllowTrustOp_trusted_account_tag  2
#define StellarAllowTrustOp_asset_type_tag       3
#define StellarAllowTrustOp_asset_code_tag       4
#define StellarAllowTrustOp_is_authorized_tag    5
#define StellarAssetType_type_tag                1
#define StellarAssetType_code_tag                2
#define StellarAssetType_issuer_tag              3
#define StellarBumpSequenceOp_source_account_tag 1
#define StellarBumpSequenceOp_bump_to_tag        2
#define StellarCreateAccountOp_source_account_tag 1
#define StellarCreateAccountOp_new_account_tag   2
#define StellarCreateAccountOp_starting_balance_tag 3
#define StellarGetAddress_address_n_tag          1
#define StellarGetAddress_show_display_tag       2
#define StellarManageDataOp_source_account_tag   1
#define StellarManageDataOp_key_tag              2
#define StellarManageDataOp_value_tag            3
#define StellarSetOptionsOp_source_account_tag   1
#define StellarSetOptionsOp_inflation_destination_account_tag 2
#define StellarSetOptionsOp_clear_flags_tag      3
#define StellarSetOptionsOp_set_flags_tag        4
#define StellarSetOptionsOp_master_weight_tag    5
#define StellarSetOptionsOp_low_threshold_tag    6
#define StellarSetOptionsOp_medium_threshold_tag 7
#define StellarSetOptionsOp_high_threshold_tag   8
#define StellarSetOptionsOp_home_domain_tag      9
#define StellarSetOptionsOp_signer_type_tag      10
#define StellarSetOptionsOp_signer_key_tag       11
#define StellarSetOptionsOp_signer_weight_tag    12
#define StellarSignTx_address_n_tag              2
#define StellarSignTx_network_passphrase_tag     3
#define StellarSignTx_source_account_tag         4
#define StellarSignTx_fee_tag                    5
#define StellarSignTx_sequence_number_tag        6
#define StellarSignTx_timebounds_start_tag       8
#define StellarSignTx_timebounds_end_tag         9
#define StellarSignTx_memo_type_tag              10
#define StellarSignTx_memo_text_tag              11
#define StellarSignTx_memo_id_tag                12
#define StellarSignTx_memo_hash_tag              13
#define StellarSignTx_num_operations_tag         14
#define StellarSignedTx_public_key_tag           1
#define StellarSignedTx_signature_tag            2
#define StellarChangeTrustOp_source_account_tag  1
#define StellarChangeTrustOp_asset_tag           2
#define StellarChangeTrustOp_limit_tag           3
#define StellarCreatePassiveOfferOp_source_account_tag 1
#define StellarCreatePassiveOfferOp_selling_asset_tag 2
#define StellarCreatePassiveOfferOp_buying_asset_tag 3
#define StellarCreatePassiveOfferOp_amount_tag   4
#define StellarCreatePassiveOfferOp_price_n_tag  5
#define StellarCreatePassiveOfferOp_price_d_tag  6
#define StellarManageOfferOp_source_account_tag  1
#define StellarManageOfferOp_selling_asset_tag   2
#define StellarManageOfferOp_buying_asset_tag    3
#define StellarManageOfferOp_amount_tag          4
#define StellarManageOfferOp_price_n_tag         5
#define StellarManageOfferOp_price_d_tag         6
#define StellarManageOfferOp_offer_id_tag        7
#define StellarPathPaymentOp_source_account_tag  1
#define StellarPathPaymentOp_send_asset_tag      2
#define StellarPathPaymentOp_send_max_tag        3
#define StellarPathPaymentOp_destination_account_tag 4
#define StellarPathPaymentOp_destination_asset_tag 5
#define StellarPathPaymentOp_destination_amount_tag 6
#define StellarPathPaymentOp_paths_tag           7
#define StellarPaymentOp_source_account_tag      1
#define StellarPaymentOp_destination_account_tag 2
#define StellarPaymentOp_asset_tag               3
#define StellarPaymentOp_amount_tag              4

/* Struct field encoding specification for nanopb */
extern const pb_field_t StellarAssetType_fields[4];
extern const pb_field_t StellarGetAddress_fields[3];
extern const pb_field_t StellarAddress_fields[2];
extern const pb_field_t StellarSignTx_fields[13];
extern const pb_field_t StellarTxOpRequest_fields[1];
extern const pb_field_t StellarPaymentOp_fields[5];
extern const pb_field_t StellarCreateAccountOp_fields[4];
extern const pb_field_t StellarPathPaymentOp_fields[8];
extern const pb_field_t StellarManageOfferOp_fields[8];
extern const pb_field_t StellarCreatePassiveOfferOp_fields[7];
extern const pb_field_t StellarSetOptionsOp_fields[13];
extern const pb_field_t StellarChangeTrustOp_fields[4];
extern const pb_field_t StellarAllowTrustOp_fields[6];
extern const pb_field_t StellarAccountMergeOp_fields[3];
extern const pb_field_t StellarManageDataOp_fields[4];
extern const pb_field_t StellarBumpSequenceOp_fields[3];
extern const pb_field_t StellarSignedTx_fields[3];

/* Maximum encoded size of messages (where known) */
#define StellarAssetType_size                    80
#define StellarGetAddress_size                   62
#define StellarAddress_size                      59
#define StellarSignTx_size                       1263
#define StellarTxOpRequest_size                  0
#define StellarPaymentOp_size                    211
#define StellarCreateAccountOp_size              129
#define StellarPathPaymentOp_size                714
#define StellarManageOfferOp_size                257
#define StellarCreatePassiveOfferOp_size         246
#define StellarSetOptionsOp_size                 235
#define StellarChangeTrustOp_size                152
#define StellarAllowTrustOp_size                 145
#define StellarAccountMergeOp_size               118
#define StellarManageDataOp_size                 193
#define StellarBumpSequenceOp_size               70
#define StellarSignedTx_size                     100

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define MESSAGES_STELLAR_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
