/*
 * Registrar protocol messages
 *
 * Copyright (C) 2009, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: reg_protomsg.h,v 1.6 2009/11/17 07:32:27 Exp $
 */

#ifndef _WPS_MSG_H
#define _WPS_MSG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <reg_prototlv.h>


/* Message Structures */


/* Message M1 */
typedef struct {
	CTlvVersion version;
	CTlvMsgType msgType;
	CTlvUuid uuid;
	CTlvMacAddr macAddr;
	CTlvEnrolleeNonce enrolleeNonce;
	CTlvPublicKey publicKey;
	CTlvAuthTypeFlags authTypeFlags;
	CTlvEncrTypeFlags encrTypeFlags;
	CTlvConnTypeFlags connTypeFlags;
	CTlvConfigMethods configMethods;
	CTlvScState scState;
	CTlvManufacturer manufacturer;
	CTlvModelName modelName;
	CTlvModelNumber modelNumber;
	CTlvSerialNum serialNumber;
	CTlvPrimDeviceType primDeviceType;
	CTlvDeviceName deviceName;
	CTlvRfBand rfBand;
	CTlvAssocState assocState;
	CTlvDevicePwdId devPwdId;
	CTlvConfigError configError;
	CTlvOsVersion osVersion;
} WpsM1;

/* Message M2 */
typedef struct {
	CTlvVersion version;
	CTlvMsgType msgType;
	CTlvEnrolleeNonce enrolleeNonce;
	CTlvRegistrarNonce registrarNonce;
	CTlvUuid uuid;
	CTlvPublicKey publicKey;
	CTlvAuthTypeFlags authTypeFlags;
	CTlvEncrTypeFlags encrTypeFlags;
	CTlvConnTypeFlags connTypeFlags;
	CTlvConfigMethods configMethods;
	CTlvManufacturer manufacturer;
	CTlvModelName modelName;
	CTlvModelNumber modelNumber;
	CTlvSerialNum serialNumber;
	CTlvPrimDeviceType primDeviceType;
	CTlvDeviceName deviceName;
	CTlvRfBand rfBand;
	CTlvAssocState assocState;
	CTlvConfigError configError;
	CTlvDevicePwdId devPwdId;
	CTlvOsVersion osVersion;
	CTlvEncrSettings encrSettings;
	CTlvAuthenticator authenticator;
} WpsM2;

/* Message M2D */
typedef struct {
	CTlvVersion version;
	CTlvMsgType msgType;
	CTlvEnrolleeNonce enrolleeNonce;
	CTlvRegistrarNonce registrarNonce;
	CTlvUuid uuid;
	CTlvAuthTypeFlags authTypeFlags;
	CTlvEncrTypeFlags encrTypeFlags;
	CTlvConnTypeFlags connTypeFlags;
	CTlvConfigMethods configMethods;
	CTlvManufacturer manufacturer;
	CTlvModelName modelName;
	CTlvModelNumber modelNumber;
	CTlvSerialNum serialNumber;
	CTlvPrimDeviceType primDeviceType;
	CTlvDeviceName deviceName;
	CTlvRfBand rfBand;
	CTlvAssocState assocState;
	CTlvConfigError configError;
	CTlvDevicePwdId devPwdId;
} WpsM2D;

/* Message M3 */
typedef struct {
	CTlvVersion version;
	CTlvMsgType msgType;
	CTlvRegistrarNonce registrarNonce;
	CTlvHash eHash1;
	CTlvHash eHash2;
	CTlvAuthenticator authenticator;
} WpsM3;

/* Message M4 */
typedef struct {
	CTlvVersion version;
	CTlvMsgType msgType;
	CTlvEnrolleeNonce enrolleeNonce;
	CTlvHash rHash1;
	CTlvHash rHash2;
	CTlvEncrSettings encrSettings;
	CTlvAuthenticator authenticator;
} WpsM4;

/* Message M5 */
typedef struct {
	CTlvVersion version;
	CTlvMsgType msgType;
	CTlvRegistrarNonce registrarNonce;
	CTlvEncrSettings encrSettings;
	CTlvAuthenticator authenticator;
} WpsM5;

/* Message M6 */
typedef struct {
	CTlvVersion version;
	CTlvMsgType msgType;
	CTlvEnrolleeNonce enrolleeNonce;
	CTlvEncrSettings encrSettings;
	CTlvAuthenticator authenticator;
} WpsM6;

/* Message M7 */
typedef struct {
	CTlvVersion version;
	CTlvMsgType msgType;
	CTlvRegistrarNonce registrarNonce;
	CTlvEncrSettings encrSettings;
	CTlvX509CertReq x509CertReq;
	CTlvAuthenticator authenticator;
} WpsM7;

/* Message M8 */
typedef struct {
	CTlvVersion version;
	CTlvMsgType msgType;
	CTlvEnrolleeNonce enrolleeNonce;
	CTlvEncrSettings encrSettings;
	CTlvX509Cert x509Cert;
	CTlvAuthenticator authenticator;
} WpsM8;

/* ACK and DONE Messages */
typedef struct {
	CTlvVersion version;
	CTlvMsgType msgType;
	CTlvEnrolleeNonce enrolleeNonce;
	CTlvRegistrarNonce registrarNonce;
} WpsACK, WpsDone;

/* NACK Message */
typedef struct {
	CTlvVersion version;
	CTlvMsgType msgType;
	CTlvEnrolleeNonce enrolleeNonce;
	CTlvRegistrarNonce registrarNonce;
	CTlvConfigError configError;
} WpsNACK;

/* Encrypted settings for various messages */

/*
 * M4, M5, M6 - contain only Nonce and vendor extension
 * this structure doesn't allocate dyamic memory
 * doesn't need a free function at the moment.
 */
typedef struct {
	CTlvNonce nonce; /* could be RS1, ES1 or RS2 */
	CTlvAuthenticator keyWrapAuth; /* reuse Authenticator data struct */
} TlvEsNonce;

/* M7 */
/* NOTE : this structure MUST be freed using reg_msg_m7enr_del */
typedef struct {
	CTlvNonce nonce; /* ES2 */
	CTlvIdentityProof idProof;
	CTlvAuthenticator keyWrapAuth; /* reuse Authenticator data struct */
} TlvEsM7Enr;

/* NOTE : this structure MUST be freed using reg_msg_m7ap_del */
typedef struct {
	CTlvNonce nonce; /* ES2 */
	CTlvSsid ssid;
	CTlvMacAddr macAddr;
	CTlvAuthType authType;
	CTlvEncrType encrType;
	LPLIST nwKeyIndex;
	LPLIST nwKey;
	CTlvWEPTransmitKey wepIdx;
	CTlvAuthenticator keyWrapAuth; /* reuse Authenticator data struct */
} CTlvEsM7Ap;

/* M8 */
/* NOTE : this structure MUST be freed using reg_msg_m8ap_del */
typedef struct {
	CTlvNwIndex nwIndex;
	CTlvSsid ssid;
	CTlvAuthType authType;
	CTlvEncrType encrType;
	LPLIST nwKeyIndex;
	LPLIST nwKey;
	CTlvMacAddr macAddr;
	CTlvNewPwd new_pwd;
	CTlvDevicePwdId pwdId;
	CTlvWEPTransmitKey wepIdx;
	CTlvAuthenticator keyWrapAuth; /* reuse Authenticator data struct */
} CTlvEsM8Ap;

typedef struct {
	LPLIST credential;
	CTlvNewPwd new_pwd;
	CTlvDevicePwdId pwdId;
	CTlvAuthenticator keyWrapAuth; /* reuse Authenticator data struct */
} CTlvEsM8Sta;

void reg_msg_init(void *m, int type);
void reg_msg_nonce_parse(TlvEsNonce *t, uint16 theType, BufferObj *theBuf, BufferObj *authKey);
void reg_msg_nonce_write(TlvEsNonce *t, BufferObj *theBuf, BufferObj *authKey);
TlvEsM7Enr * reg_msg_m7enr_new();
void reg_msg_m7enr_del(TlvEsM7Enr *t, bool content_only);
uint32 reg_msg_m7enr_parse(TlvEsM7Enr *t, BufferObj *theBuf, BufferObj *authKey, bool allocate);
void reg_msg_m7enr_write(TlvEsM7Enr *t, BufferObj *theBuf, BufferObj *authKey);
CTlvEsM7Ap * reg_msg_m7ap_new();
void reg_msg_m7ap_del(CTlvEsM7Ap *tlv, bool content_only);
uint32 reg_msg_m7ap_parse(CTlvEsM7Ap *tlv, BufferObj *theBuf, BufferObj *authKey, bool allocate);
void reg_msg_m7ap_write(CTlvEsM7Ap *tlv, BufferObj *theBuf, BufferObj *authKey);
CTlvEsM8Ap *reg_msg_m8ap_new();
void reg_msg_m8ap_del(CTlvEsM8Ap *t, bool content_only);
void reg_msg_m8ap_parse(CTlvEsM8Ap *t, BufferObj *theBuf, BufferObj *authKey, bool allocate);
void reg_msg_m8ap_write(CTlvEsM8Ap *t, BufferObj *theBuf, BufferObj *authKey);
CTlvEsM8Sta *reg_msg_m8sta_new();
void reg_msg_m8sta_del(CTlvEsM8Sta *t, bool content_only);
void reg_msg_m8sta_parse(CTlvEsM8Sta *t, BufferObj *theBuf, BufferObj *authKey, bool allocate);
void reg_msg_m8sta_write(CTlvEsM8Sta *t, BufferObj *theBuf);
void reg_msg_m8sta_write_cred(CTlvEsM8Sta *t, BufferObj *theBuf);
void reg_msg_m8sta_write_key(CTlvEsM8Sta *t, BufferObj *theBuf, BufferObj *authKey);


#ifdef __cplusplus
}
#endif

#endif /* _WPS_MSG_H */