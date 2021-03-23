// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#ifndef __AUTH_H__
#define __AUTH_H__

#include <MFRC522.h>

class TAuth {
public:
	TAuth();

	bool begin();

	bool authorized();

private:
	MFRC522 mfrc522;
};

extern TAuth Auth;

#endif /* __AUTH_H__ */