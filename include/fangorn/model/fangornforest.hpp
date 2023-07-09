#pragma once

#include "fangorn/fangornbase.hpp"

enum FangornForestItemType {
    FOREST_ITEM_GENERIC_TEXT_DOC,
    FOREST_ITEM_SUBFOREST,
    FOREST_ITEM_BT_DOC,
    FOREST_ITEM_BT_LOG,
    FOREST_ITEM_BT_WORKSPACE,
    FOREST_ITEM_DEBUG_CONF
};


class FangornForestItem {
    public:
    DEF_SHARED_PTR_TYPES(FangornForestItem);
    FangornForestItem(const std::string& path, FangornForestItemType type);
    
    const FangornForestItemType getItemType() const;
    const std::string getPath() const;
    const std::string getDisplayName() const;
    virtual void changePath(const std::string& newPath);

    private:
    const FangornForestItemType type;
    std::string path;
};


class FangornForest : public FangornForestItem {
    public:
    DEF_SHARED_PTR_TYPES(FangornForest);

    //loading
    void loadFromDirectory(const std::string& directory);
    const std::string getDirectory() const;

    //item management
    const std::list<FangornForestItem::SharedPtr> getItems() const;
    void addItem(FangornForestItem::SharedPtr item);
    void removeItem(FangornForestItem::SharedPtr item);

    private:
    std::string currentDirectory;
    std::list<FangornForestItem::SharedPtr> items;
};
