// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#include <LoRa.h>
#include <mbedtls/aes.h>

#include "Comm.h"

TComm Comm;

TComm::TComm() : m_heartbeat_period(10000), m_magic(0xDEADBEEF) {

}

bool TComm::begin(int ss, int reset, int dio0) {
	LoRa.setPins(ss, reset, dio0);

	/* Set timeout for read ops */
	LoRa.setTimeout(1000);

	if (!LoRa.begin(433E6)) {
		Serial.println("Starting SX1278 failed!");
		return false;
	}

  	return true;
}

void TComm::setMagic(uint32_t magic) {
	m_magic = magic;
}

void TComm::setEncryptionKey(const unsigned char *key) {
	memcpy(m_key, key, 16);
}

void TComm::setHeartbeatPeriod(unsigned long period) {
	m_heartbeat_period = period;
}

void TComm::sendPacket(const TComm::Packet *pkt) {
	mbedtls_aes_context aes;
	const unsigned char *p;
	int nchunks, chunk;

	if (sizeof(Packet) % 16 != 0) {
		Serial.println("Packet size must be a multiple of 16!");
		return;
	}

 	mbedtls_aes_init(&aes);

	mbedtls_aes_setkey_enc(&aes, m_key, 128);

	LoRa.beginPacket();

	nchunks = sizeof(Packet) / 16;
	for (chunk = 0, p = (const unsigned char *)pkt;
	     chunk < nchunks;
	     chunk++, p += 16) {
		unsigned char encbuff[16];

		mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, p, encbuff);

  		LoRa.write(encbuff, sizeof(encbuff));
	}

	LoRa.endPacket();

	mbedtls_aes_free(&aes);
}

bool TComm::readPacket(TComm::Packet *pkt) {
	mbedtls_aes_context aes;
	unsigned char *p;
	int nchunks, chunk;
	int packetSize;

	packetSize = LoRa.parsePacket();
	if (packetSize <= 0) {
		return false;
	}

	if (packetSize % 16 != 0 || sizeof(Packet) % 16 != 0) {
		Serial.println("Packet size must be a multiple of 16!");
		return false;
	}

 	mbedtls_aes_init(&aes);

	mbedtls_aes_setkey_enc(&aes, m_key, 128);

	nchunks = sizeof(Packet) / 16;
	for (chunk = 0, p = (unsigned char *)pkt;
	     chunk < nchunks;
	     chunk++, p += 16) {
		unsigned char encbuff[16];
		size_t n;

		n = LoRa.available();
		if (n <= 0) {
			mbedtls_aes_free(&aes);
			return false;
		}

		n = LoRa.readBytes(encbuff, 16);
		if (n < 16) {
			mbedtls_aes_free(&aes);
			return false;
		}

		mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, encbuff, p);
	}

	mbedtls_aes_free(&aes);

	if (pkt->magic != m_magic) {
		return false;
	}

	return true;
}

bool TComm::heartbeat(const char *state) {
	static unsigned long time_beacon = 0;

	if (millis() - time_beacon > m_heartbeat_period) {
		static uint64_t counter = 0;
		Packet pkt;

		memset(&pkt, 0, sizeof(pkt));

		pkt.magic = m_magic;
		pkt.type = HEARTBEAT;
		pkt.heartbeat.counter = counter++;
		snprintf(pkt.heartbeat.state, 16, "%s", state);

		sendPacket(&pkt);

		time_beacon = millis();
		return true;
	}
	return false;
}

void TComm::auth(uint32_t uid, bool authorized) {
	Packet pkt;

	memset(&pkt, 0, sizeof(pkt));

	pkt.magic = m_magic;
	pkt.type = AUTH;
	pkt.auth.uid = uid;
	pkt.auth.authorized = authorized;
	sendPacket(&pkt);
}

void TComm::door(bool open) {
	Packet pkt;

	memset(&pkt, 0, sizeof(pkt));

	pkt.magic = m_magic;
	pkt.type = DOOR;
	pkt.door.open = open;
	sendPacket(&pkt);
}

void TComm::alarm() {
	Packet pkt;

	memset(&pkt, 0, sizeof(pkt));

	pkt.magic = m_magic;
	pkt.type = ALARM;
	sendPacket(&pkt);
}