#include "fangorn/plugin/fangornmanager.hpp"
#include "fangorn/util/fangornlogging.hpp"

FangornManager::FangornManager()
{

}


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
