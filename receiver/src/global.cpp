// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#include <Arduino.h>
#include <Log.h>

#include "config.h"
#include "global.h"

TGlobal Global;

TGlobal::TGlobal() {
        heartbeatTs = 0;
        state = "BOOT";
        alarmTriggered = false;
}