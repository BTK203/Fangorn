#include "fangorn/model/fangornbtdocument.hpp"


FangornBtDocument::FangornBtDocument(const std::string& path)
 : FangornForestItem(path, FangornForestItemType::FOREST_ITEM_BT_DOC),
   xmlDoc(QString::fromStdString(path)) { }


void FangornBtDocument::changePath(const std::string& newPath)
{
    rename(getPath().c_str(), newPath.c_str());
    FangornForestItem::changePath(newPath);
}


const std::list<std::string> FangornBtDocument::getTreeNames()
{
    return trees;
}


FangornBtNode::SharedPtr FangornBtDocument::createTreeWithName(const std::string& name)
{
    return nullptr;
}


bool FangornBtDocument::hasTreeByName(const std::string& name)
{
    for(auto it = trees.begin(); it != trees.end(); it++)
    {
        if(*it == name)
        {
            return true;
        }
    }

    return false;
}


void FangornBtDocument::removeTreeByName(const std::string& name)
{

}


FangornBtNode::SharedPtr FangornBtDocument::getTreeByName(const std::string& name) const 
{
    return nullptr;
}
