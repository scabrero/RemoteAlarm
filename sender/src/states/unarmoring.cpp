// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#include <Buzzer.h>

#include "unarmoring.h"
#include "config.h"
#include "global.h"
#include "auth.h"

TUnarmoring::TUnarmoring() {

}

const char *TUnarmoring::name() const {
	return STATE_UNARMORING_STR;
}

bool TUnarmoring::timedout() {
	return (millis() - m_timestamp > UNARMORING_TIMEOUT_MS);
}

void TUnarmoring::enter() {
	m_timestamp = millis();
	Buzzer.warning();
}

void TUnarmoring::loop() {
	if (Auth.authorized()) {
		Global.setState(&Global.StateUnarmored);
		return;
	}

	if (timedout()) {
		Global.setState(&Global.StateAlarm);
		return;
	}
}

void TUnarmoring::exit() {
	Buzzer.shutup();
}