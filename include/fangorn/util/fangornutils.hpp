#pragma once

#include "fangorn/fangornbase.hpp"
#include "fangorn/model/fangornbtnode.hpp"
#include "fangorn/util/fangornlogging.hpp"

//
// Helpful macros
//
#define SUB(...) __VA_ARGS__

#define FANGORN_GENERIC_ASSERT(condition, ...) \
    do \
    { \
        if(!(condition)) \
        { \
            FANGORN_FATAL(__VA_ARGS__); \
            FANGORN_FATAL("^ ^ ^ Failed: " #condition); \
            throw std::runtime_error(#condition); \
        } \
    } while(0)

#define FANGORN_ASSERT_BASE(type, base) \
    FANGORN_GENERIC_ASSERT(SUB(std::is_base_of<base, type>::value), "Expected " #type " (which is %s) to be a child-class of " #base, typeid(type).name())

//
// Helpful functions
//

int findInArgs(int argc, char **argv, const char *arg);
std::string nodePathToString(std::vector<FangornBtNode::ConstSharedPtr> path);
QString fileToString(const QString& pathh);
