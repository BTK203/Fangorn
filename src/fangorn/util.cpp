#include "fangorn/fangornutils.hpp"
#include <string.h>

int findInArgs(int argc, char **argv, const char *arg) {
    for(int i = 0; i < argc; i++) {
        if(strcmp(argv[i], arg) == 0) {
            return i;
        }
    }

    return -1;
}
