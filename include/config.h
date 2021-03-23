// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define PIN_LED			LED_BUILTIN

#define PIN_SX1278_SS		5
#define PIN_SX1278_RST		4
#define PIN_SX1278_DIO0		15

#define PIN_RC522_SS		21
#define PIN_RC522_RST		22

#define PIN_DOOR		16
#define PIN_LIGHT		17
#define PIN_BUZZER		13

#define STATE_ALARM_STR         "ALARM"
#define STATE_ARMORED_STR       "ARMORED"
#define STATE_ARMORING_STR      "ARMORING"
#define STATE_UNARMORED_STR     "UNARMORED"
#define STATE_UNARMORING_STR    "UNARMORING"

#define HEARTBEAT_TIMEOUT_THRESHOLD_SEC         10      /* 10 seconds */
#define WIFI_RECONNECT_PERIOD_SEC               30      /* 30 seconds */
#define HEARTBEAT_PERIOD_MS	1000
#define ARMORING_TIMEOUT_MS	10000
#define UNARMORING_TIMEOUT_MS	10000

#define EMAIL_PERIOD            60000 //3600000L        /* 1 hour */
#define EMAIL_LOG_PERIOD        60000 //86400000L       /* 1 day */
#define EMAIL_LOG_HOUR          1              /* 9:00 PM */

#define NTP_SERVER      "pool.ntp.org"
#define GMT_OFFSET      3600
#define DST_OFFSET      0

#endif