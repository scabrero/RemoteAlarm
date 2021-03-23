// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#include <Arduino.h>
#include <SPI.h>
#include <Buzzer.h>
#include <Comm.h>
#include <WiFi.h>
#include <Log.h>
#include <UMailer.h>

#include "config.h"
#include "keys.h"

static void deadLed() {
	while (1) {
		delay(100);
		digitalWrite(PIN_LED, HIGH);
		delay(100);
		digitalWrite(PIN_LED, LOW);
	}
}

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
	Log.info("WiFi connected");

	configTime(GMT_OFFSET, DST_OFFSET, NTP_SERVER);
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
	Log.info("WiFi disconnected: %d", info.disconnected.reason);
	WiFi.reconnect();
}

void WiFiStationGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
	Log.info("WiFi got IP %s", ip4addr_ntoa(&info.got_ip.ip_info.ip));
}

void setup() {
	Serial.begin(115200);
	SPI.begin();

	pinMode(PIN_LED, OUTPUT);
	digitalWrite(PIN_LED, LOW);

	if (!Buzzer.begin(PIN_BUZZER)) {
                deadLed();
	}

	Comm.setMagic(COMM_MAGIC);
	Comm.setEncryptionKey(comm_key);
	if (!Comm.begin(PIN_SX1278_SS, PIN_SX1278_RST, PIN_SX1278_DIO0)) {
		deadLed();
	}

	WiFi.disconnect();
	WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_STA_CONNECTED);
  	WiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);
	WiFi.onEvent(WiFiStationGotIP, SYSTEM_EVENT_STA_GOT_IP);
        WiFi.mode(WIFI_STA);
  	WiFi.begin(WIFI_SSID, WIFI_PASS);

	for (int i=0; i<20 && WiFi.status() != WL_CONNECTED; i++) {
    		Serial.print(".");
		digitalWrite(PIN_LED, HIGH);
    		delay(250);
		digitalWrite(PIN_LED, LOW);
		delay(250);
  	}

	UMailer.setTimeout(10000);
	UMailer.setClientName("alarma.orica.es");
	UMailer.begin(SMTP_SERVER);
}