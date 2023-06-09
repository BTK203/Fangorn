#pragma once

//
// This file contains includes needed by specialized fangorn headers and definitions
//

//
// INCLUDES
//
#include <stdio.h>
#include <behaviortree_cpp/action_node.h>


//
//FEATURE SWTICHES
//
#define FEATURE_ENABLE 1
#define FEATURE_DISABLE 0

#define FEATURE_PRINT_DEBUG (FEATURE_ENABLE)
#define FEATURE_TESTFLAG    (FEATURE_ENABLE)


//
// LOGGING MACRO CONFIGURATION
//

//FANGORN_INFO
#ifdef RCLCPP_INFO
    #define FANGORN_LOGGER rclcpp::get_logger("fangorn")
    #define FANGORN_INFO(...) RCLCPP_INFO(FANGORN_LOGGER, __VA_ARGS__)
#else
    #define __FANGORN_PRINT_FMTTD(fmt, ...) printf(fmt "%s\n", __VA_ARGS__)
    #define FANGORN_INFO(...) __FANGORN_PRINT_FMTTD(__VA_ARGS__, "")
#endif

//FANGORN_DEBUG
#define FANGORN_DEBUG(...) \
    do { \
        if(FEATURE_PRINT_DEBUG == FEATURE_ENABLE) { \
            FANGORN_INFO(__VA_ARGS__); \
        } \
    } while(0)

