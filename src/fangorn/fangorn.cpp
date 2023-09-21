#include "fangorn/fangorncommon.hpp"
#include "fangorn/control/fangorncontroller.hpp"
#include "fangorn/plugin/fangornmanager.hpp"
#include <unistd.h>


class ExampleController : public FangornController {
    public:
    DEF_SMART_PTR_TYPES(ExampleController);
    ExampleController(FangornView::SharedPtr context)
     : FangornController() { };
};


template<typename T>
void printType()
{
    FANGORN_INFO("%s", typeid(T).name());
}


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

    FangornPlugin plugin = FANGORN_CREATE_PLUGIN(
        FANGORN_PACKAGE_CONTROLLER(ExampleController)
    );

    FangornManager manager;
    manager.registerPlugin(plugin);
    manager.start<ExampleController>(argc, argv);
    return 0;
}
