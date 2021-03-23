// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#include <Buzzer.h>
#include "config.h"

TBuzzer Buzzer;

static void IRAM_ATTR onTimer() {
	Buzzer.timerHandler();
}

TBuzzer::TBuzzer() : timer(NULL) {

}

bool TBuzzer::begin(uint8_t pin) {
	m_pin = pin;

	pinMode(m_pin, OUTPUT);

	/*
	 * Use the first timer of 4
	 * 1 tick take 1/(80MHZ/80) = 1us so we set divider 80 and count up
	 */
	timer = timerBegin(0, 80, true);
	timerAttachInterrupt(timer, &onTimer, true);

	return true;
}

void TBuzzer::notification() {
	stopTimer();
	setFrecuency(15);
	setDuration(250);
	startTimer();
}

void TBuzzer::warning() {
	stopTimer();
	setFrecuency(2);
	setDuration(ULONG_MAX);
	startTimer();
}

void TBuzzer::alarm() {
	stopTimer();
	setFrecuency(20);
	setDuration(ULONG_MAX);
	startTimer();
}

void TBuzzer::shutup() {
	stopTimer();
}

void TBuzzer::setFrecuency(unsigned long hz) {
	timerAlarmWrite(timer, 1000000l / hz, true);
}

void TBuzzer::setDuration(unsigned long ms) {
	duration = ms;
}

void TBuzzer::startTimer() {
	start = millis();
	value = true;
	digitalWrite(m_pin, value);
	timerAlarmEnable(timer);
}

void TBuzzer::stopTimer() {
	timerAlarmDisable(timer);
	value = false;
	digitalWrite(m_pin, value);
}

void TBuzzer::timerHandler() {
	if (millis() - start > duration) {
		stopTimer();
		return;
	}

	value = !value;
  	digitalWrite(m_pin, value);
}