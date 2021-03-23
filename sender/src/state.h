// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#ifndef __TSTATE_H__
#define __TSTATE_H__

class TState {
public:
	TState() {}

	virtual const char *name() const = 0;

	virtual void enter() {}
	virtual void loop() = 0;
	virtual void exit() {}
};

#endif /* __TSTATE_H__ */