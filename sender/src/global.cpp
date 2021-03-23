// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#include <Log.h>

#include "config.h"
#include "global.h"
#include "state.h"

TGlobal Global;

TGlobal::TGlobal() : m_state(&StateUnarmored) {

}

TState *TGlobal::getState() {
	return m_state;
}

void TGlobal::setState(TState *state) {
	m_state->exit();

	Log.info("Change state to %s", state->name());
	m_state = state;

	m_state->enter();
}