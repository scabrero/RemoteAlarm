// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "states/alarm.h"
#include "states/armored.h"
#include "states/armoring.h"
#include "states/unarmored.h"
#include "states/unarmoring.h"

class TGlobal {
public:
	TGlobal();

	void setState(TState *state);
	TState *getState();

	TAlarm StateAlarm;
	TArmored StateArmored;
	TArmoring StateArmoring;
	TUnarmored StateUnarmored;
	TUnarmoring StateUnarmoring;

private:
	TState *m_state;
};

extern TGlobal Global;

#endif /* __GLOBAL_H__ */