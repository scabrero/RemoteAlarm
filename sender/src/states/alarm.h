// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#ifndef __STATE_ALARM_H__
#define __STATE_ALARM_H__

#include "state.h"

class TAlarm : public TState {
public:
	TAlarm();

	virtual const char *name() const;

	virtual void enter();
	virtual void loop();
	virtual void exit();
};

#endif /* __STATE_ALARM_H__ */