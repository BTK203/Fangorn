#pragma once

#include "fangorn/view/fangornview.hpp"
#include "fangorn/fangornbase.hpp"


class FangornController {
    public:
    DEF_SMART_PTR_TYPES(FangornController);
    FangornController();
    
    static std::vector<std::string> dependencies();
    FangornView::SharedPtr getView();

    virtual void initialize(const std::string& dep, FangornView::SharedPtr context);

    protected:
    FangornView::SharedPtr view;
};

typedef std::function<FangornController::SharedPtr(FangornView::SharedPtr)> FangornControllerBuilder;
