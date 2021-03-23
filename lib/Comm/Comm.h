// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#ifndef __BEACON_H__
#define __BEACON_H__

#include <Arduino.h>

class TComm {
public:
	enum PacketType {
		HEARTBEAT,
		AUTH,
		DOOR,
		ALARM,
	};

	struct Packet {
		uint32_t magic;
		PacketType type;
		union {
			struct {
				char state[16];
				uint64_t counter;
			} heartbeat;
			struct {
				uint32_t uid;
				bool authorized;
			} auth;
			struct {
				bool open;
			} door;
		};
		/* Pad struct size to a multiple of 16 bytes for AES-ECB encryption */
	} __attribute__ ((packed));

	TComm();

	bool begin(int ss, int reset, int dio0);

	void setMagic(uint32_t magic);
	void setEncryptionKey(const unsigned char *key);

	void setHeartbeatPeriod(unsigned long period);
	bool heartbeat(const char *state);
	void auth(uint32_t uid, bool authorized);
	void door(bool open);
	void alarm();

	void sendPacket(const Packet *pkt);
	bool readPacket(Packet *pkt);

private:
	unsigned long m_heartbeat_period;
	unsigned char m_key[16];
	uint32_t m_magic;
};

extern TComm Comm;

#endif /* __BEACON__H__ */