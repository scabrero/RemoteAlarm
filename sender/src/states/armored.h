// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#ifndef __STATE_ARMORED_H__
#define __STATE_ARMORED_H__

#include "state.h"

class TArmored : public TState {
public:
	TArmored();

	virtual const char *name() const;

	virtual void enter();
	virtual void loop();
};

#endif /* __STATE_ARMORED_H__ */