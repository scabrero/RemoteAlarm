// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#include <Arduino.h>
#include <SPI.h>
#include <Buzzer.h>
#include <Comm.h>

#include "config.h"
#include "global.h"
#include "auth.h"
#include "states/armoring.h"
#include "keys.h"

static void deadLed() {
	while (1) {
		delay(100);
		digitalWrite(PIN_LED, HIGH);
		delay(100);
		digitalWrite(PIN_LED, LOW);
	}
}

void setup() {
	Serial.begin(115200);
	SPI.begin();

	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_DOOR, INPUT_PULLUP);
	pinMode(PIN_LIGHT, OUTPUT);

	digitalWrite(PIN_LED, LOW);
	digitalWrite(PIN_LIGHT, HIGH);

	if (!Buzzer.begin(PIN_BUZZER)) {
		deadLed();
	}

	if (!Auth.begin()) {
		deadLed();
	}

	Comm.setMagic(COMM_MAGIC);
	Comm.setEncryptionKey(comm_key);
	Comm.setHeartbeatPeriod(HEARTBEAT_PERIOD_MS);
	if (!Comm.begin(PIN_SX1278_SS, PIN_SX1278_RST, PIN_SX1278_DIO0)) {
		deadLed();
	}

	/*
	 * Start in armoring state. If main power goes down
	 * the alarm will rearm when power is restored
	 */
	Global.setState(&Global.StateArmoring);
}