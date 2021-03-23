// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#include <Buzzer.h>

#include "armoring.h"
#include "config.h"
#include "global.h"
#include "auth.h"

TArmoring::TArmoring() {

}

const char *TArmoring::name() const {
	return STATE_ARMORING_STR;
}

bool TArmoring::timedout() {
	return (millis() - m_timestamp > ARMORING_TIMEOUT_MS);
}

void TArmoring::enter() {
	m_timestamp = millis();
	Buzzer.warning();
}

void TArmoring::loop() {
	if (Auth.authorized()) {
		Global.setState(&Global.StateUnarmored);
		return;
	}

	if (!timedout()) {
		return;
	}

	Global.setState(&Global.StateArmored);
}

void TArmoring::exit() {
	Buzzer.shutup();
}