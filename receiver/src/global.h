// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <UMailer.h>
#include <deque>

class TGlobal {
public:
	TGlobal();

        std::deque<TUMailer::Message> mailq;
        time_t heartbeatTs;
        String state;
        bool alarmTriggered;
};

extern TGlobal Global;

#endif /* __GLOBAL_H__ */