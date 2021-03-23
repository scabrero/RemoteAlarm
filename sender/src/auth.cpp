// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#include <Comm.h>
#include <Log.h>

#include "auth.h"
#include "config.h"
#include "keys.h"

TAuth::TAuth() : mfrc522(PIN_RC522_SS, PIN_RC522_RST) {

}

bool TAuth::begin() {
	mfrc522.PCD_Init();
	if (!mfrc522.PCD_PerformSelfTest()) {
		Log.error("MFRC522 self test failed!");
		return false;
	}

	mfrc522.PCD_Init();
	mfrc522.PCD_DumpVersionToSerial();

	return true;
}

bool TAuth::authorized() {
	MFRC522::StatusCode status;
	uint32_t uid;
	bool ret = false;

	if (!mfrc522.PICC_IsNewCardPresent()) {
		return false;
	}

	if (!mfrc522.PICC_ReadCardSerial()) {
		return false;
	}

  	status = mfrc522.PICC_HaltA();
	if (status != MFRC522::StatusCode::STATUS_OK) {
		return false;
	}

	uid =  (uint32_t) mfrc522.uid.uidByte[0] << 24;
	uid |= (uint32_t) mfrc522.uid.uidByte[1] << 16;
	uid |= (uint32_t) mfrc522.uid.uidByte[2] << 8;
	uid |= (uint32_t) mfrc522.uid.uidByte[3];

	/* Authentication only by UID for now */
	size_t n = sizeof(authorized_uids) / sizeof(uint32_t);
	for (int i=0; i<n; i++) {
		if (uid == authorized_uids[i]) {
			ret = true;
			break;
		}
	}

	Log.info("%s 0x%08X", ret ? "Authorized" : "Unauthorized", uid);
	Comm.auth(uid, ret);

	return ret;
}

TAuth Auth;