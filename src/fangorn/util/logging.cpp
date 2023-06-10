#include <cstdarg>
#include <iomanip>
#include "fangorn/fangornlogging.hpp"

char 
    msgBuf[FANGORN_MAX_LOG_MSG_LENGTH],
    auxBuf[FANGORN_MAX_LOG_MSG_LENGTH];

void __fangornLogMsgClear(void) {
    memset(msgBuf, 0, sizeof(msgBuf));
    memset(auxBuf, 0, sizeof(auxBuf));
}

void __fangornLogMsgAppend(const char *fmt, ...) {
    va_list va;
    va_start(va, fmt);
    vsprintf(auxBuf, fmt, va);
    va_end(va);
    strcat(msgBuf, auxBuf);
}

const char *__fangornLogMsg(void) {
    return msgBuf;
}