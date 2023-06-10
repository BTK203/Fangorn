#pragma once
#include "fangorn/fangornbase.hpp"


//
// Logging header for Fangorn
//

static const char *FANGORN_LOGLVL_NAMES[] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

static const std::chrono::time_point __fangornStartTime = std::chrono::system_clock::now();


//
// LOGGING MACROS
//
#define COLOR_DEBUG_NUM 4
#define COLOR_INFO_NUM  7
#define COLOR_WARN_NUM  3
#define COLOR_ERROR_NUM 1
#define COLOR_FAULT_NUM 1
#define COLOR_FATAL_NUM 5

#define COLOR_BRIGHT 9
#define COLOR_DARK   3

#define xstr(s) str(s)
#define str(s) #s

#define COLOR_TAG_DEBUG "\033[1;" xstr(COLOR_DARK) xstr(COLOR_DEBUG_NUM) "m"
#define COLOR_TAG_INFO  "\033[1;" xstr(COLOR_DARK) xstr(COLOR_INFO_NUM) "m"
#define COLOR_TAG_WARN  "\033[1;" xstr(COLOR_DARK) xstr(COLOR_WARN_NUM) "m"
#define COLOR_TAG_ERROR "\033[1;" xstr(COLOR_DARK) xstr(COLOR_ERROR_NUM) "m"
#define COLOR_TAG_FAULT "\033[1;38:5:166m"
#define COLOR_TAG_FATAL "\033[1;" xstr(COLOR_DARK) xstr(COLOR_FATAL_NUM) "m"

#define COLOR_MSG_DEBUG "\033[0;" xstr(COLOR_BRIGHT) xstr(COLOR_DEBUG_NUM) "m"
#define COLOR_MSG_INFO  "\033[0;" xstr(COLOR_BRIGHT) xstr(COLOR_INFO_NUM) "m"
#define COLOR_MSG_WARN  "\033[0;" xstr(COLOR_BRIGHT) xstr(COLOR_WARN_NUM) "m"
#define COLOR_MSG_ERROR "\033[0;" xstr(COLOR_BRIGHT) xstr(COLOR_ERROR_NUM) "m"
#define COLOR_MSG_FAULT "\033[0;38:5:202m"
#define COLOR_MSG_FATAL "\033[0;" xstr(COLOR_BRIGHT) xstr(COLOR_FATAL_NUM) "m"

//macro for shortened filename (instead of /home/.../src/fangorn/xyz.cpp, display src/fangorn/xyz.cpp)
#define __PROJECT_FILENAME  (strstr(__FILE__, "src/fangorn")     ? strstr(__FILE__, "src/fangorn")     : \
                            (strstr(__FILE__, "include/fangorn") ? strstr(__FILE__, "include/fangorn") : "unknown"))

#define __FG_LOG_CLEAR                             __fangornLogMsgClear()
#define __FG_LOG_APPEND(...)                       __fangornLogMsgAppend(__VA_ARGS__)

//this is a goofy way to use the power of format strings to make the variadic arg optional.
//anything invoking this macro must provide an empty string as the last argument.
#define __FG_LOG_MSG_APPEND_WITH_SUFFIX(fmt, ...)  __fangornLogMsgAppend("[" fmt "]%s ", __VA_ARGS__)

//log prefix configuration macros
#define __FG_LOG_PRE_CUSTOM(...)                   __FG_LOG_MSG_APPEND_WITH_SUFFIX(__VA_ARGS__, "")
#define __FG_LOG_PRE_NAME                          __FG_LOG_PRE_CUSTOM("fangorn")
#define __FG_LOG_PRE_LOGLVL(lvl)                   __FG_LOG_PRE_CUSTOM("%s", FANGORN_LOGLVL_NAMES[lvl])
#define __FG_LOG_PRE_FILE                          __FG_LOG_PRE_CUSTOM("%s", __PROJECT_FILENAME)
#define __FG_LOG_PRE_FILE_LINE                     __FG_LOG_PRE_CUSTOM("%s : %d", __PROJECT_FILENAME, __LINE__)
#define __FG_LOG_PRE_TIME                          __FG_LOG_PRE_CUSTOM("%d", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - __fangornStartTime).count())

//log color configuration macro
#define __FG_LOG_COLOR(color) \
    do { \
        if(FANGORN_FEATURE_LOG_COLORS == FEATURE_ENABLE) { \
            __FG_LOG_APPEND(color); \
        } \
    } while(0)

//printf logger. can be used with __FANGORN_LOG
#define __PRINTF_LOGGER(msg) printf("%s\n", msg)

//assembles and prints a log message.
#define __FANGORN_LOG(log, loglvl, colors, prefixes, ...) \
    do { \
        if(loglvl >= FANGORN_LOG_LEVEL) { \
            __FG_LOG_CLEAR; \
            colors; \
            prefixes; \
            __FG_LOG_APPEND(__VA_ARGS__); \
            log(__fangornLogMsg()); \
        } \
    } while(0)

#if (defined(RCLCPP_DEBUG) \
  && defined(RCLCPP_INFO) \
  && defined(RCLCPP_WARN) \
  && defined(RCLCPP_ERROR) \
  && defined(RCLCPP_FATAL))
    #warning "RCLCPP Logging is not supported yet." 
    //TODO: define loggers as their respective rclcpp calls  
#else
    #define __DEBUG_LOGGER __PRINTF_LOGGER
    #define __INFO_LOGGER __PRINTF_LOGGER
    #define __WARN_LOGGER __PRINTF_LOGGER
    #define __ERROR_LOGGER __PRINTF_LOGGER
    #define __FATAL_LOGGER __PRINTF_LOGGER

    #define __DEBUG_PREFIXES __FG_LOG_PRE_NAME; __FG_LOG_PRE_LOGLVL(FangornLogLevel::DEBUG); __FG_LOG_PRE_FILE; __FG_LOG_PRE_TIME
    #define __INFO_PREFIXES __FG_LOG_PRE_NAME; __FG_LOG_PRE_LOGLVL(FangornLogLevel::INFO)
    #define __WARN_PREFIXES __FG_LOG_PRE_NAME; __FG_LOG_PRE_LOGLVL(FangornLogLevel::WARN); __FG_LOG_PRE_FILE
    #define __ERROR_PREFIXES __FG_LOG_PRE_NAME; __FG_LOG_PRE_LOGLVL(FangornLogLevel::ERROR); __FG_LOG_PRE_FILE_LINE
    #define __FATAL_PREFIXES __FG_LOG_PRE_NAME; __FG_LOG_PRE_LOGLVL(FangornLogLevel::FATAL); __FG_LOG_PRE_FILE_LINE; __FG_LOG_PRE_TIME
#endif

#define __DEBUG_COLORS __FG_LOG_COLOR(COLOR_TAG_DEBUG); __FG_LOG_COLOR(COLOR_MSG_DEBUG)
#define __INFO_COLORS __FG_LOG_COLOR(COLOR_TAG_INFO); __FG_LOG_COLOR(COLOR_MSG_INFO)
#define __WARN_COLORS __FG_LOG_COLOR(COLOR_TAG_WARN); __FG_LOG_COLOR(COLOR_MSG_WARN)
#define __ERROR_COLORS __FG_LOG_COLOR(COLOR_TAG_ERROR); __FG_LOG_COLOR(COLOR_MSG_ERROR)
#define __FATAL_COLORS __FG_LOG_COLOR(COLOR_TAG_FATAL); __FG_LOG_COLOR(COLOR_MSG_FATAL)

#define FANGORN_DEBUG(...) __FANGORN_LOG(__DEBUG_LOGGER, FangornLogLevel::DEBUG, __DEBUG_COLORS, __DEBUG_PREFIXES, __VA_ARGS__)
#define FANGORN_INFO(...) __FANGORN_LOG(__INFO_LOGGER, FangornLogLevel::INFO, __INFO_COLORS, __INFO_PREFIXES, __VA_ARGS__)
#define FANGORN_WARN(...) __FANGORN_LOG(__WARN_LOGGER, FangornLogLevel::WARN, __WARN_COLORS, __WARN_PREFIXES, __VA_ARGS__)
#define FANGORN_ERROR(...) __FANGORN_LOG(__ERROR_LOGGER, FangornLogLevel::ERROR, __ERROR_COLORS, __ERROR_PREFIXES, __VA_ARGS__)
#define FANGORN_FATAL(...) __FANGORN_LOG(__FATAL_LOGGER, FangornLogLevel::FATAL, __FATAL_COLORS, __FATAL_PREFIXES, __VA_ARGS__)

void __fangornLogMsgClear(void);
void __fangornLogMsgAppend(const char *fmt, ...);
const char *__fangornLogMsg(void);
