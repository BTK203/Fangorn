#include <cstdarg>
#include <iomanip>
#include "fangorn/util/fangornlogging.hpp"

char 
    msgBuf[FANGORN_MAX_LOG_MSG_LENGTH],
    auxBuf[FANGORN_MAX_LOG_MSG_LENGTH];

TestLabelFunc testLblFunc;
bool testLblFuncInitialized = false;


void __fangornLogMsgClear(void)
{
    memset(msgBuf, 0, sizeof(msgBuf));
    memset(auxBuf, 0, sizeof(auxBuf));
}


void __fangornLogMsgAppend(const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    vsprintf(auxBuf, fmt, va);
    va_end(va);
    strcat(msgBuf, auxBuf);
}


const char *__fangornLogMsg(void)
{
    return msgBuf;
}


#if FANGORN_FEATURE_TEST_LABEL == FEATURE_ENABLE

void __fangornRegisterTestLabelFunc(TestLabelFunc func)
{
    if(testLblFuncInitialized) {
        FANGORN_ERROR("Test Label Function already initialized! Cannot initialize it twice.");
        return;
    }

    testLblFunc = func;
}


void __fangornSetTestLabel(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsprintf(msgBuf, fmt, args);
    va_end(args);
    testLblFunc(msgBuf);
}

#endif
