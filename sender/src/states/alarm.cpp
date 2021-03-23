// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#include <Buzzer.h>
#include <Comm.h>

#include "alarm.h"
#include "config.h"
#include "global.h"
#include "auth.h"

TAlarm::TAlarm() {

}

const char *TAlarm::name() const {
	return STATE_ALARM_STR;
}

void TAlarm::enter() {
	Comm.alarm();
	Buzzer.alarm();
}

void TAlarm::loop() {
	if (Auth.authorized()) {
		Global.setState(&Global.StateUnarmored);
		return;
	}
}

void TAlarm::exit() {
	Buzzer.shutup();
}