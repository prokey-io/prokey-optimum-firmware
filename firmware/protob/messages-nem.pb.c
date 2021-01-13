/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9.3 */

#include "messages-nem.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t NEMGetAddress_fields[4] = {
    PB_FIELD(  1, UINT32  , REPEATED, STATIC  , FIRST, NEMGetAddress, address_n, address_n, 0),
    PB_FIELD(  2, UINT32  , OPTIONAL, STATIC  , OTHER, NEMGetAddress, network, address_n, 0),
    PB_FIELD(  3, BOOL    , OPTIONAL, STATIC  , OTHER, NEMGetAddress, show_display, network, 0),
    PB_LAST_FIELD
};

const pb_field_t NEMAddress_fields[2] = {
    PB_FIELD(  1, STRING  , REQUIRED, STATIC  , FIRST, NEMAddress, address, address, 0),
    PB_LAST_FIELD
};

const pb_field_t NEMSignTx_fields[10] = {
    PB_FIELD(  1, MESSAGE , OPTIONAL, STATIC  , FIRST, NEMSignTx, transaction, transaction, &NEMTransactionCommon_fields),
    PB_FIELD(  2, MESSAGE , OPTIONAL, STATIC  , OTHER, NEMSignTx, multisig, transaction, &NEMTransactionCommon_fields),
    PB_FIELD(  3, MESSAGE , OPTIONAL, STATIC  , OTHER, NEMSignTx, transfer, multisig, &NEMTransfer_fields),
    PB_FIELD(  4, BOOL    , OPTIONAL, STATIC  , OTHER, NEMSignTx, cosigning, transfer, 0),
    PB_FIELD(  5, MESSAGE , OPTIONAL, STATIC  , OTHER, NEMSignTx, provision_namespace, cosigning, &NEMProvisionNamespace_fields),
    PB_FIELD(  6, MESSAGE , OPTIONAL, STATIC  , OTHER, NEMSignTx, mosaic_creation, provision_namespace, &NEMMosaicCreation_fields),
    PB_FIELD(  7, MESSAGE , OPTIONAL, STATIC  , OTHER, NEMSignTx, supply_change, mosaic_creation, &NEMMosaicSupplyChange_fields),
    PB_FIELD(  8, MESSAGE , OPTIONAL, STATIC  , OTHER, NEMSignTx, aggregate_modification, supply_change, &NEMAggregateModification_fields),
    PB_FIELD(  9, MESSAGE , OPTIONAL, STATIC  , OTHER, NEMSignTx, importance_transfer, aggregate_modification, &NEMImportanceTransfer_fields),
    PB_LAST_FIELD
};

const pb_field_t NEMTransactionCommon_fields[7] = {
    PB_FIELD(  1, UINT32  , REPEATED, STATIC  , FIRST, NEMTransactionCommon, address_n, address_n, 0),
    PB_FIELD(  2, UINT32  , OPTIONAL, STATIC  , OTHER, NEMTransactionCommon, network, address_n, 0),
    PB_FIELD(  3, UINT32  , OPTIONAL, STATIC  , OTHER, NEMTransactionCommon, timestamp, network, 0),
    PB_FIELD(  4, UINT64  , OPTIONAL, STATIC  , OTHER, NEMTransactionCommon, fee, timestamp, 0),
    PB_FIELD(  5, UINT32  , OPTIONAL, STATIC  , OTHER, NEMTransactionCommon, deadline, fee, 0),
    PB_FIELD(  6, BYTES   , OPTIONAL, STATIC  , OTHER, NEMTransactionCommon, signer, deadline, 0),
    PB_LAST_FIELD
};

const pb_field_t NEMTransfer_fields[6] = {
    PB_FIELD(  1, STRING  , OPTIONAL, STATIC  , FIRST, NEMTransfer, recipient, recipient, 0),
    PB_FIELD(  2, UINT64  , OPTIONAL, STATIC  , OTHER, NEMTransfer, amount, recipient, 0),
    PB_FIELD(  3, BYTES   , OPTIONAL, STATIC  , OTHER, NEMTransfer, payload, amount, 0),
    PB_FIELD(  4, BYTES   , OPTIONAL, STATIC  , OTHER, NEMTransfer, public_key, payload, 0),
    PB_FIELD(  5, MESSAGE , REPEATED, STATIC  , OTHER, NEMTransfer, mosaics, public_key, &NEMMosaic_fields),
    PB_LAST_FIELD
};

const pb_field_t NEMMosaic_fields[4] = {
    PB_FIELD(  1, STRING  , OPTIONAL, STATIC  , FIRST, NEMMosaic, namespace, namespace, 0),
    PB_FIELD(  2, STRING  , OPTIONAL, STATIC  , OTHER, NEMMosaic, mosaic, namespace, 0),
    PB_FIELD(  3, UINT64  , OPTIONAL, STATIC  , OTHER, NEMMosaic, quantity, mosaic, 0),
    PB_LAST_FIELD
};

const pb_field_t NEMProvisionNamespace_fields[5] = {
    PB_FIELD(  1, STRING  , OPTIONAL, STATIC  , FIRST, NEMProvisionNamespace, namespace, namespace, 0),
    PB_FIELD(  2, STRING  , OPTIONAL, STATIC  , OTHER, NEMProvisionNamespace, parent, namespace, 0),
    PB_FIELD(  3, STRING  , OPTIONAL, STATIC  , OTHER, NEMProvisionNamespace, sink, parent, 0),
    PB_FIELD(  4, UINT64  , OPTIONAL, STATIC  , OTHER, NEMProvisionNamespace, fee, sink, 0),
    PB_LAST_FIELD
};

const pb_field_t NEMMosaicCreation_fields[4] = {
    PB_FIELD(  1, MESSAGE , OPTIONAL, STATIC  , FIRST, NEMMosaicCreation, definition, definition, &NEMMosaicDefinition_fields),
    PB_FIELD(  2, STRING  , OPTIONAL, STATIC  , OTHER, NEMMosaicCreation, sink, definition, 0),
    PB_FIELD(  3, UINT64  , OPTIONAL, STATIC  , OTHER, NEMMosaicCreation, fee, sink, 0),
    PB_LAST_FIELD
};

const pb_field_t NEMMosaicDefinition_fields[16] = {
    PB_FIELD(  1, STRING  , OPTIONAL, STATIC  , FIRST, NEMMosaicDefinition, name, name, 0),
    PB_FIELD(  2, STRING  , OPTIONAL, STATIC  , OTHER, NEMMosaicDefinition, ticker, name, 0),
    PB_FIELD(  3, STRING  , OPTIONAL, STATIC  , OTHER, NEMMosaicDefinition, namespace, ticker, 0),
    PB_FIELD(  4, STRING  , OPTIONAL, STATIC  , OTHER, NEMMosaicDefinition, mosaic, namespace, 0),
    PB_FIELD(  5, UINT32  , OPTIONAL, STATIC  , OTHER, NEMMosaicDefinition, divisibility, mosaic, 0),
    PB_FIELD(  6, UENUM   , OPTIONAL, STATIC  , OTHER, NEMMosaicDefinition, levy, divisibility, 0),
    PB_FIELD(  7, UINT64  , OPTIONAL, STATIC  , OTHER, NEMMosaicDefinition, fee, levy, 0),
    PB_FIELD(  8, STRING  , OPTIONAL, STATIC  , OTHER, NEMMosaicDefinition, levy_address, fee, 0),
    PB_FIELD(  9, STRING  , OPTIONAL, STATIC  , OTHER, NEMMosaicDefinition, levy_namespace, levy_address, 0),
    PB_FIELD( 10, STRING  , OPTIONAL, STATIC  , OTHER, NEMMosaicDefinition, levy_mosaic, levy_namespace, 0),
    PB_FIELD( 11, UINT64  , OPTIONAL, STATIC  , OTHER, NEMMosaicDefinition, supply, levy_mosaic, 0),
    PB_FIELD( 12, BOOL    , OPTIONAL, STATIC  , OTHER, NEMMosaicDefinition, mutable_supply, supply, 0),
    PB_FIELD( 13, BOOL    , OPTIONAL, STATIC  , OTHER, NEMMosaicDefinition, transferable, mutable_supply, 0),
    PB_FIELD( 14, STRING  , OPTIONAL, STATIC  , OTHER, NEMMosaicDefinition, description, transferable, 0),
    PB_FIELD( 15, UINT32  , REPEATED, STATIC  , OTHER, NEMMosaicDefinition, networks, description, 0),
    PB_LAST_FIELD
};

const pb_field_t NEMMosaicSupplyChange_fields[5] = {
    PB_FIELD(  1, STRING  , OPTIONAL, STATIC  , FIRST, NEMMosaicSupplyChange, namespace, namespace, 0),
    PB_FIELD(  2, STRING  , OPTIONAL, STATIC  , OTHER, NEMMosaicSupplyChange, mosaic, namespace, 0),
    PB_FIELD(  3, UENUM   , OPTIONAL, STATIC  , OTHER, NEMMosaicSupplyChange, type, mosaic, 0),
    PB_FIELD(  4, UINT64  , OPTIONAL, STATIC  , OTHER, NEMMosaicSupplyChange, delta, type, 0),
    PB_LAST_FIELD
};

const pb_field_t NEMAggregateModification_fields[3] = {
    PB_FIELD(  1, MESSAGE , REPEATED, STATIC  , FIRST, NEMAggregateModification, modifications, modifications, &NEMCosignatoryModification_fields),
    PB_FIELD(  2, SINT32  , OPTIONAL, STATIC  , OTHER, NEMAggregateModification, relative_change, modifications, 0),
    PB_LAST_FIELD
};

const pb_field_t NEMCosignatoryModification_fields[3] = {
    PB_FIELD(  1, UENUM   , OPTIONAL, STATIC  , FIRST, NEMCosignatoryModification, type, type, 0),
    PB_FIELD(  2, BYTES   , OPTIONAL, STATIC  , OTHER, NEMCosignatoryModification, public_key, type, 0),
    PB_LAST_FIELD
};

const pb_field_t NEMImportanceTransfer_fields[3] = {
    PB_FIELD(  1, UENUM   , OPTIONAL, STATIC  , FIRST, NEMImportanceTransfer, mode, mode, 0),
    PB_FIELD(  2, BYTES   , OPTIONAL, STATIC  , OTHER, NEMImportanceTransfer, public_key, mode, 0),
    PB_LAST_FIELD
};

const pb_field_t NEMSignedTx_fields[3] = {
    PB_FIELD(  1, BYTES   , OPTIONAL, STATIC  , FIRST, NEMSignedTx, data, data, 0),
    PB_FIELD(  2, BYTES   , OPTIONAL, STATIC  , OTHER, NEMSignedTx, signature, data, 0),
    PB_LAST_FIELD
};

const pb_field_t NEMDecryptMessage_fields[5] = {
    PB_FIELD(  1, UINT32  , REPEATED, STATIC  , FIRST, NEMDecryptMessage, address_n, address_n, 0),
    PB_FIELD(  2, UINT32  , OPTIONAL, STATIC  , OTHER, NEMDecryptMessage, network, address_n, 0),
    PB_FIELD(  3, BYTES   , OPTIONAL, STATIC  , OTHER, NEMDecryptMessage, public_key, network, 0),
    PB_FIELD(  4, BYTES   , OPTIONAL, STATIC  , OTHER, NEMDecryptMessage, payload, public_key, 0),
    PB_LAST_FIELD
};

const pb_field_t NEMDecryptedMessage_fields[2] = {
    PB_FIELD(  1, BYTES   , OPTIONAL, STATIC  , FIRST, NEMDecryptedMessage, payload, payload, 0),
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
PB_STATIC_ASSERT((pb_membersize(NEMSignTx, transaction) < 65536 && pb_membersize(NEMSignTx, multisig) < 65536 && pb_membersize(NEMSignTx, transfer) < 65536 && pb_membersize(NEMSignTx, provision_namespace) < 65536 && pb_membersize(NEMSignTx, mosaic_creation) < 65536 && pb_membersize(NEMSignTx, supply_change) < 65536 && pb_membersize(NEMSignTx, aggregate_modification) < 65536 && pb_membersize(NEMSignTx, importance_transfer) < 65536 && pb_membersize(NEMTransfer, payload) < 65536 && pb_membersize(NEMTransfer, mosaics[0]) < 65536 && pb_membersize(NEMMosaicCreation, definition) < 65536 && pb_membersize(NEMAggregateModification, modifications[0]) < 65536 && pb_membersize(NEMSignedTx, data) < 65536 && pb_membersize(NEMDecryptMessage, payload) < 65536 && pb_membersize(NEMDecryptedMessage, payload) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_NEMGetAddress_NEMAddress_NEMSignTx_NEMTransactionCommon_NEMTransfer_NEMMosaic_NEMProvisionNamespace_NEMMosaicCreation_NEMMosaicDefinition_NEMMosaicSupplyChange_NEMAggregateModification_NEMCosignatoryModification_NEMImportanceTransfer_NEMSignedTx_NEMDecryptMessage_NEMDecryptedMessage)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
#error Field descriptor for NEMSignedTx.data is too large. Define PB_FIELD_16BIT to fix this.
#endif


/* @@protoc_insertion_point(eof) */
