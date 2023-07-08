#pragma once

#include "fangorn/fangornbase.hpp"
#include "fangorn/model/fangornbtnode.hpp"

int findInArgs(int argc, char **argv, const char *arg);
std::string nodePathToString(std::vector<FangornBTNode::ConstSharedPtr> path);
