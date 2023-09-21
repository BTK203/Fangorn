#pragma once

#include "fangorn/fangornbase.hpp"
#include "fangorn/model/fangornbtnode.hpp"
#include "fangorn/model/fangornforest.hpp"

// struct FangornBTNodeModel {
//     BT::NodeType type;
//     std::string registrationId;
//     BT::PortsList ports;
// };

class FangornBtDocument : public FangornForestItem {
    public:
    DEF_SMART_PTR_TYPES(FangornBtDocument);
    FangornBtDocument(const std::string& path);

    //document management
    void changePath(const std::string& newPath) override;
    void saveDocument() const;

    //tree management
    const std::list<std::string> getTreeNames();
    FangornBtNode::SharedPtr createTreeWithName(const std::string& name);
    bool hasTreeByName(const std::string& name);
    void removeTreeByName(const std::string& name);
    FangornBtNode::SharedPtr getTreeByName(const std::string& name) const;

    private:
    QDomDocument xmlDoc;
    std::list<std::string> trees;
};