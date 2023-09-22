#include "fangorn/plugin/fangornmanager.hpp"
#include "fangorn/util/fangornlogging.hpp"

FangornManager::FangornManager()
{ }


void FangornManager::registerPlugin(FangornPlugin plugin)
{   
    for(auto it = plugin.begin(); it != plugin.end(); it++)
    {
        FANGORN_GENERIC_ASSERT(!hasControllerTypeByName(it->first), "Controller with type %s is already registered!", it->first.c_str());
        FANGORN_DEBUG("Registered controller: %s", it->first.c_str());
    }

    plugins.push_back(plugin);
}


bool FangornManager::hasControllerTypeByName(const std::string& name)
{
    for(auto listIt = plugins.begin(); listIt != plugins.end(); listIt++)
    {
        for(auto mapIt = listIt->begin(); mapIt != listIt->end(); listIt++)
        {
            if(mapIt->first == name)
            {
                return true;
            }
        }
    }

    return false;
}


void FangornManager::notifyDependencyStarted(const std::string& dep, std::map<std::string, FangornController::SharedPtr>& initialized)
{
    for(auto plugin : plugins)
    {
        for(auto controllerPair : plugin) // {controllerType, packagedController}
        {
            FangornPackagedController package = controllerPair.second;
            if(std::find(package.dependencies.begin(), package.dependencies.end(), dep) != package.dependencies.end())
            {
                //dep contained within controller dependencies, so start this controller
                start(package.name, dep, initialized);
            }
        }
    }
}


void FangornManager::start(const std::string& type, const std::string& dep, std::map<std::string, FangornController::SharedPtr>& initialized)
{
    FANGORN_DEBUG("Start %s from dep %s", type.c_str(), dep.c_str());
}

