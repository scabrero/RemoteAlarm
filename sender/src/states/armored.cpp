// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#include <Buzzer.h>
#include <Comm.h>

#include "armored.h"
#include "config.h"
#include "global.h"

TArmored::TArmored() {

}

const char *TArmored::name() const {
	return STATE_ARMORED_STR;
}

void TArmored::enter() {
	Buzzer.notification();
}

void TArmored::loop() {
	bool doorOpen = digitalRead(PIN_DOOR);

	if (!doorOpen) {
		digitalWrite(PIN_LIGHT, HIGH);
		return;
	}
	Comm.door(true);

	digitalWrite(PIN_LIGHT, LOW);

	Global.setState(&Global.StateUnarmoring);
}