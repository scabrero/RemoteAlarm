// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#ifndef __BUZZER_H__
#define __BUZZER_H__

#include <Arduino.h>

class TBuzzer {
public:
	TBuzzer();

	bool begin(uint8_t pin);

	void notification();
	void warning();
	void alarm();
	void shutup();

	/* Internal use, called from timer ISR */
	void timerHandler();

private:
	void setFrecuency(unsigned long hz);
	void setDuration(unsigned long ms);
	void startTimer();
	void stopTimer();

	uint8_t m_pin;
	hw_timer_t *timer;
	bool value;
	unsigned long start;
	unsigned long duration;
};

extern TBuzzer Buzzer;

#endif /* __BUZZER_H__ */