// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#ifndef __LOG_H__
#define __LOG_H__

#include <Arduino.h>

class TLog {
public:
    TLog();

    void debug(const char *fmt, ...);
	void info(const char *fmt, ...);
	void error(const char *fmt, ...);

    int count() const;
    const char *getBuffer(int idx) const;
    void clearBuffer();

private:
    static const int BuffMsgLen = 128;
    static const int BuffMsgNum = 64;

    void log(const char *lvl, bool buffer, const char *fmt, va_list args);
    char m_buffer[BuffMsgNum][BuffMsgLen];
    int m_idx;
};

extern TLog Log;

#endif /* __LOG_H__ */