#include "fangorn/model/fangornforest.hpp"

FangornForestItem::FangornForestItem(const std::string& documentName, FangornForestItemType type)
 : type(type)
{
    this->documentName = documentName;
}


const FangornForestItemType FangornForestItem::itemType() const
{
    return type;
}
