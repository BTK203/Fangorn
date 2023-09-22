#include "fangorn/fangorncommon.hpp"
#include "fangorn/control/fangorncontroller.hpp"
#include "fangorn/plugin/fangornmanager.hpp"
#include <unistd.h>

int runFangorn(int argc, char **argv)
{
    #if FANGORN_FEATURE_TESTFLAG == FEATURE_ENABLE
        if(findInArgs(argc, argv, "--test") > -1) {
            usleep(100000);
            FANGORN_DEBUG("This is a debug message!");
            FANGORN_INFO("This is an %s message!", "info");
            FANGORN_WARN("This is a warn message!");
            FANGORN_ERROR("This is an %s message!", "error");
            FANGORN_FATAL("This is a fatal message!");
            // FANGORN_DEBUG("This is a debug statement! Pi is %f", 3.1415);
            return 0;
        }
    #endif

    // FangornManager manager;
    // manager.registerPlugin(plugin);
    // manager.start<ExampleController>(argc, argv);
    return 0;
}
