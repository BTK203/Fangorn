#include "fangorn/model/fangornforest.hpp"

FangornForestItem::FangornForestItem(const std::string& itemName, FangornForestItemType type)
 : type(type)
{
    this->path = path;
}


const FangornForestItemType FangornForestItem::getItemType() const
{
    return type;
}


const std::string FangornForestItem::getPath() const 
{
    return path;
}


void FangornForestItem::changePath(const std::string& newPath)
{
    this->path = path;
}
