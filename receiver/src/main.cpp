// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#include <Arduino.h>
#include <Comm.h>
#include <Log.h>
#include <WiFi.h>
#include <UMailer.h>

#include "config.h"
#include "global.h"

static void processAlarm() {
	TUMailer::Message msg;

	if (Global.alarmTriggered) {
		/* Already notified */
		return;
	}

	Global.alarmTriggered = true;

	Log.info("Alarm triggered!!!");

	/* Queue the mail */
	msg.setFromEmail("alarma@orica.es");
	msg.setFromName("Garage alarm");
	msg.addTo("samuel@orica.es");
	msg.addTo("cristina@orica.es");
	msg.setSubject("Alarm trigeered!!!");
	msg.setBody("The alarm has been triggered.");
	msg.appendBody("Event log:");
	if (Log.count() > 0) {
		for (int i=0; i<Log.count(); i++) {
			msg.appendBody(Log.getBuffer(i));
			msg.appendBody("\r\n");
		}
	} else {
		msg.appendBody("No events");
	}

	Log.debug("Alarm email queued");
	Global.mailq.push_back(msg);
}

static void processPacketHeartbeat(uint64_t counter, const char *state) {
	Global.heartbeatTs = time(NULL);

	digitalWrite(PIN_LED, HIGH);
	delay(50);
	digitalWrite(PIN_LED, LOW);
	delay(50);

	Log.debug("HEARTBEAT 0x%" PRIx64 " %s", counter, state);

	if (Global.state.compareTo(state) == 0) {
		return;
	}

	Log.info("State change %s -> %s", Global.state, state);

	Global.state = state;

	if (Global.state.compareTo(STATE_ALARM_STR) == 0) {
		processAlarm();
	} else {
		Global.alarmTriggered = false;
	}
}

static void processPacket(const TComm::Packet &pkt) {
	switch(pkt.type) {
	case TComm::ALARM:
		processAlarm();
		break;
	case TComm::AUTH:
		Log.info("AUTH: 0x%08X %s", pkt.auth.uid, pkt.auth.authorized ? "authorized" : "unauthorized");
		break;
	case TComm::DOOR:
		Log.info("DOOR: %s", pkt.door.open ? "open" : "close");
		break;
	case TComm::HEARTBEAT:
		processPacketHeartbeat(pkt.heartbeat.counter, pkt.heartbeat.state);
		return;
	default:
		Log.error("Unknown packet type %d received", pkt.type);
		break;
	}
}

static void processHeartbeatTimeout() {
	TUMailer::Message msg;
	struct tm *tm;
	char buf[128];

	tm = localtime(&Global.heartbeatTs);
	snprintf(buf, sizeof(buf),
		"Connection lost, last contact %02d/%02d/%04d %02d:%02d:%02d",
		tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900,
		tm->tm_hour, tm->tm_min, tm->tm_sec);
	Log.info(buf);

	/* Add 24 hours to Global.heartbeatTs to do not notify again in 24 hours */
	Global.heartbeatTs += 86400;

	/* Queue the mail */
	msg.setFromEmail("alarma@orica.es");
	msg.setFromName("Garage alarm");
	msg.addTo("samuel@orica.es");
	msg.addTo("cristina@orica.es");
	msg.setSubject("Connection lost");
	msg.setBody(buf);

	Log.debug("Heartbeat timeout email queued");
	Global.mailq.push_back(msg);
}

static void processComms() {
	static time_t wifiReconnectTs = 0;
	time_t ts = time(NULL);

	if (Global.state.compareTo("BOOT") == 0) {
		return;
	}

	if (ts - Global.heartbeatTs > HEARTBEAT_TIMEOUT_THRESHOLD_SEC) {
		processHeartbeatTimeout();
		return;
	}

	if ((WiFi.status() != WL_CONNECTED)) {
		if (millis() - wifiReconnectTs > WIFI_RECONNECT_PERIOD_SEC) {
			Log.debug("Reconnecting wifi");
			WiFi.reconnect();
			wifiReconnectTs = ts;
		}
	}
}

static void processLog() {
	static unsigned long ts = 0;
	struct tm tm = { 0 };

	if (millis() - ts > EMAIL_LOG_PERIOD) {
		TUMailer::Message msg;

		if (getLocalTime(&tm)) {
			if (tm.tm_hour != EMAIL_LOG_HOUR) {
				return;
			}
		}

		Log.debug("Sending event log mail");

		msg.setFromEmail("alarma@orica.es");
		msg.setFromName("Garage alarm");
		msg.addTo("samuel@orica.es");
		msg.addTo("cristina@orica.es");
		msg.setSubject("Event log");

		if (Log.count() > 0) {
			for (int i=0; i<Log.count(); i++) {
				msg.appendBody(Log.getBuffer(i));
				msg.appendBody("\r\n");
			}
			Log.clearBuffer();
		} else {
			char buf[32];
			snprintf(buf, 32, "[%02d/%02d/%04d %02d:%02d:%02d]",
                		tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
                		tm.tm_hour, tm.tm_min, tm.tm_sec);
			msg.setBody(String(buf) + ": No events.");
		}

		Log.debug("Event log email queued");
		Global.mailq.push_back(msg);
		ts = millis();
	}
}

static void processMailQueue() {
	if (WiFi.status() != WL_CONNECTED) {
		return;
	}

	if (Global.mailq.empty()) {
		return;
	}

	Log.debug("Processing mail queue");
	UMailer.send(Global.mailq);
}

void loop() {
	TComm::Packet pkt;

	if (Comm.readPacket(&pkt)) {
		processPacket(pkt);
		return;
	}

	processComms();

	processLog();

	processMailQueue();
}