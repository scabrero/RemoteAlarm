// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#include <Arduino.h>
#include <Comm.h>
#include <Log.h>

#include "config.h"
#include "global.h"
#include "state.h"

void loop() {
	TState *s = Global.getState();

	s->loop();

	if (Comm.heartbeat(s->name())) {
  		digitalWrite(PIN_LED, HIGH);
		delay(50);
		digitalWrite(PIN_LED, LOW);
		delay(50);
	}
}