// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#include <Buzzer.h>

#include "unarmored.h"
#include "config.h"
#include "global.h"
#include "auth.h"

TUnarmored::TUnarmored() {

}

const char *TUnarmored::name() const {
	return STATE_UNARMORED_STR;
}

void TUnarmored::enter() {
	Buzzer.notification();
}

void TUnarmored::loop() {
	if (!Auth.authorized()) {
		return;
	}

	Global.setState(&Global.StateArmoring);
}