#pragma once

//
// This file contains includes needed by specialized fangorn headers and definitions
//

//
// INCLUDES
//
#include <stdio.h>
#include <chrono>
#include <list>
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


//macro to define shared pointer types
#define DEF_SMART_PTR_TYPES(t) \
    typedef std::shared_ptr<t> SharedPtr; \
    typedef std::shared_ptr<const t> ConstSharedPtr; \
    typedef std::unique_ptr<t> UniquePtr; \
    typedef std::unique_ptr<const t> ConstUniquePtr; \
    typedef std::weak_ptr<t> WeakPtr; \
    typedef std::weak_ptr<const t> ConstWeakPtr


//
// PROGRAM SETTINGS
//
#define FEATURE_ENABLE 1
#define FEATURE_DISABLE 0


#define FANGORN_FEATURE_TESTFLAG    (FEATURE_ENABLE)
#define FANGORN_FEATURE_TEST_LABEL  (FEATURE_DISABLE)

//LOGGING
#define FANGORN_LOG_LEVEL FangornLogLevel::DEBUG
#define FANGORN_MAX_LOG_MSG_LENGTH 2048
#define FANGORN_FEATURE_LOG_COLORS (FEATURE_ENABLE)
