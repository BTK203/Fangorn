#include "fangorn/model/fangornforest.hpp"


void FangornForest::loadFromDirectory(const std::string& directory)
{
    currentDirectory = directory;
}


const std::string FangornForest::getDirectory() const
{
    return currentDirectory;
}


const std::list<FangornForestItem::SharedPtr> FangornForest::getItems() const
{
    return items;
}


void FangornForest::addItem(FangornForestItem::SharedPtr item)
{
    items.push_back(item);
}


void FangornForest::removeItem(FangornForestItem::SharedPtr item)
{
    items.remove(item);
}
