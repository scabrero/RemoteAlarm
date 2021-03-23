// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#include <Arduino.h>
#include <stdarg.h>
#include <stdio.h>

#include "Log.h"

TLog Log;

TLog::TLog() {
    m_idx = 0;
}

void TLog::log(const char *lvl, bool buffer, const char *fmt, va_list args) {
    char *p = m_buffer[m_idx];
    struct tm tm = { 0 };
    size_t n, r;

    getLocalTime(&tm, 0);

    r = BuffMsgLen;
    n = snprintf(p, r, "[%02d/%02d/%04d %02d:%02d:%02d] [%s] ",
                tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
                tm.tm_hour, tm.tm_min, tm.tm_sec, lvl);
    if (n < r) {
        r -= n;
        p += n;
    }

    vsnprintf(p, r, fmt, args);

    Serial.println(m_buffer[m_idx]);

    if (!buffer) {
        return;
    }

    m_idx++;

    if (m_idx % BuffMsgNum == 0) {
        m_idx = 0;
        error("%s", "Log size rebased");
    }
}

void TLog::error(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    log("ERROR", true, fmt, ap);
    va_end(ap);
}

void TLog::info(const char *fmt, ...)
{
	va_list ap;

    va_start(ap, fmt);
    log("INFO", true, fmt, ap);
    va_end(ap);
}

void TLog::debug(const char *fmt, ...)
{
	va_list ap;

    va_start(ap, fmt);
    log("DEBUG", false, fmt, ap);
    va_end(ap);
}

int TLog::count() const {
    return m_idx;
}

const char *TLog::getBuffer(int idx) const {
    return m_buffer[idx];
}

void TLog::clearBuffer() {
    m_idx = 0;
}