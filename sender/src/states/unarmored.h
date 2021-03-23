// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#ifndef __STATE_UNARMORED_H__
#define __STATE_UNARMORED_H__

#include "state.h"

class TUnarmored : public TState {
public:
	TUnarmored();

	virtual const char *name() const;

	virtual void enter();
	virtual void loop();
};

#endif /* __STATE_UNARMORED_H__ */