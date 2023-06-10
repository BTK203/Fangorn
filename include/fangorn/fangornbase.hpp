#pragma once

//
// This file contains includes needed by specialized fangorn headers and definitions
//

//
// INCLUDES
//
#include <stdio.h>
#include <chrono>
#include <behaviortree_cpp/action_node.h>

//
// NEEDED STUFF
//
enum FangornLogLevel {
    DEBUG = 0,
    INFO,
    WARN,
    ERROR,
    FATAL
};


//
// PROGRAM SETTINGS
//
#define FEATURE_ENABLE 1
#define FEATURE_DISABLE 0


#define FANGORN_FEATURE_TESTFLAG    (FEATURE_ENABLE)


//LOGGING
#define FANGORN_LOG_LEVEL FangornLogLevel::DEBUG
#define FANGORN_MAX_LOG_MSG_LENGTH 2048
#define FANGORN_FEATURE_LOG_COLORS (FEATURE_ENABLE)
