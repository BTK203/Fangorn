#pragma once

#include "fangorn/fangorncommon.hpp"
#include "fangorn/control/fangorncontroller.hpp"

template<typename T>
class FangornGenericController : public FangornController {
    public:
    FangornGenericController(QWidget *context)
     : FangornController()
    {
        view = std::make_shared<T>(context);
    }

    void initialize(const std::string& dep, FangornView::SharedPtr context) override
    { }

    std::list<std::string> dependencies() override
    {
        return {};
    }
};
