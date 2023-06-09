#include "fangorn/fangornutils.hpp"
#include "fangorn/view/fangornmainwindow.h"
#include <QApplication>


int runFangorn(int argc, char **argv) {
    #if FEATURE_TESTFLAG == FEATURE_ENABLE
        if(findInArgs(argc, argv, "--test") > -1) {
            FANGORN_INFO("Hello World!");
            FANGORN_DEBUG("This is a debug statement!");
            return 0;
        }
    #endif

    QApplication a(argc, argv);
    FangornMainWindow w;
    w.show();
    return a.exec();
}
