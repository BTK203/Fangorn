#include "fangorn/control/fangorncontroller.hpp"
#include "fangorn/util/fangornlogging.hpp"

FangornController::FangornController()
{
}

    
void FangornController::initialize(const std::string& dep, FangornView::SharedPtr context)
{
    FANGORN_WARN("FangornController initialize() function not overridden. Override me!");
}


std::list<std::string> FangornController::dependencies()
{
    FANGORN_WARN("FangornController dependencies() function not overridden. Override me!");
    return {};
}
