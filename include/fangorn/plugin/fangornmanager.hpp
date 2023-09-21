#pragma once
#include "fangorn/fangornbase.hpp"
#include "fangorn/control/fangorncontroller.hpp"
#include "fangorn/view/fangornmainwindow.hpp"
#include "fangorn/view/fangornmainwidget.hpp"
#include "fangorn/control/fangorngenericcontroller.hpp"
#include <QApplication>
#include <QLayout>

#define FANGORN_CREATE_PLUGIN(...) { __VA_ARGS__ }
#define FANGORN_PACKAGE_CONTROLLER(type) \
    { \
        typeid(type).name(), \
        [] (FangornView::SharedPtr context) { \
            return std::make_shared<type>(context); \
        } \
    }

typedef std::map<std::string, FangornControllerBuilder> FangornPlugin;

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

    template<typename Controller>
    Controller getInitializedController()
    {
        FANGORN_GENERIC_ASSERT(hasControllerType<Controller>(), "Controller type %s is not initialized!", typeid(Controller).name());
    }
    
    template<typename Controller>
    void start(FangornView::SharedPtr context)
    {
        
    }

    template<typename Controller>
    int start(int argc, char **argv)
    {
        FANGORN_ASSERT_BASE(Controller, FangornController);
        FANGORN_GENERIC_ASSERT(hasControllerType<Controller>(), "Expected controller %s to be registered!", typeid(Controller).name());
        
        QApplication a(argc, argv);
        FangornMainWindow mw;


        // initializedControllers.insert({ "mainwindow",
        //     std::make_shared<FangornGenericController<FangornMainWidget>>(mw.getMainPanel())
        // });

        // mw.getMainPanel()->layout()->addWidget(initializedControllers["mainwindow"]->getView());

        std::map<std::string, FangornController::SharedPtr> initializedControllers;

        FangornController::SharedPtr cnt = std::make_shared<FangornGenericController<FangornMainWidget>>(mw.getMainPanel());
        mw.getMainPanel()->layout()->addWidget(cnt->getView().get());
        initializedControllers["mainwindow"] = cnt;
        
        mw.show();
        int result = a.exec();
        return result;
    }

    private:
    std::list<FangornPlugin> plugins;
};
