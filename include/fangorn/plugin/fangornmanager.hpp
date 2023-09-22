#pragma once
#include "fangorn/fangornbase.hpp"
#include "fangorn/control/fangorncontroller.hpp"
#include "fangorn/view/fangornmainwindow.hpp"
#include "fangorn/view/fangornmainwidget.hpp"
#include "fangorn/control/fangorngenericcontroller.hpp"
#include <QApplication>
#include <QLayout>

struct FangornPackagedController {
    std::string name;
    FangornControllerBuilder builder;
    std::vector<std::string> dependencies;
};

inline FangornPackagedController __fangornPackageController(
    const std::string& name,
    const FangornControllerBuilder& builder,
    const std::vector<std::string>& dependencies)
{
    FangornPackagedController __controller;
    __controller.name = name;
    __controller.builder = builder;
    __controller.dependencies = dependencies;
    return __controller;
}

#define FANGORN_CREATE_PLUGIN(...) { __VA_ARGS__ }
#define FANGORN_PACKAGE_CONTROLLER(type) \
    { \
        typeid(type).name(), \
        __fangornPackageController( \
            typeid(type).name(), \
            [] (FangornView::SharedPtr context) { \
                return std::make_shared<type>(context); \
            }, \
            type::dependencies() \
        ), \
    }

typedef std::map<std::string, FangornPackagedController> FangornPlugin;

class FangornManager {
    public:
    DEF_SMART_PTR_TYPES(FangornManager);

    FangornManager();
    void registerPlugin(FangornPlugin plugin);
    bool hasControllerTypeByName(const std::string& name);

    template<typename Controller>
    bool hasControllerType()
    {
        return hasControllerTypeByName(typeid(Controller).name());
    }

    void notifyDependencyStarted(const std::string& dep, std::map<std::string, FangornController::SharedPtr>& initialized);
    void start(const std::string& type, const std::string& dep, std::map<std::string, FangornController::SharedPtr>& initialized);

    template<typename Controller>
    int start(int argc, char **argv)
    {
        FANGORN_ASSERT_BASE(Controller, FangornController);
        FANGORN_GENERIC_ASSERT(hasControllerType<Controller>(), "Expected controller %s to be registered!", typeid(Controller).name());
        
        QApplication a(argc, argv);
        FangornMainWindow mw;

        std::map<std::string, FangornController::SharedPtr> initializedControllers;

        FangornController::SharedPtr cnt = std::make_shared<FangornGenericController<FangornMainWidget>>(mw.getMainPanel());
        mw.getMainPanel()->layout()->addWidget(cnt->getView().get());
        initializedControllers["mainwindow"] = cnt;

        notifyDependencyStarted("mainwindow", initializedControllers);
        
        mw.show();
        int result = a.exec();
        return result;
    }

    private:
    std::list<FangornPlugin> plugins;
    std::map<std::string, std::list<std::string>> controllerDeps;
};
