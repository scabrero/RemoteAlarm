// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#ifndef __STATE_UNARMORING_H__
#define __STATE_UNARMORING_H__

#include "state.h"

class TUnarmoring : public TState {
public:
	TUnarmoring();

	virtual const char *name() const;

	virtual void enter();
	virtual void loop();
	virtual void exit();

private:
	bool timedout();

	unsigned long m_timestamp;
};

#endif /* __STATE_UNARMORING_H__ */