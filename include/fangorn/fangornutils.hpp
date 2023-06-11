#pragma once

#include "fangorn/fangornbase.hpp"
#include "fangorn/fangorncomponents.hpp"

int findInArgs(int argc, char **argv, const char *arg);
std::string nodePathToString(std::vector<FangornBTNode::ConstSharedPtr> path);
