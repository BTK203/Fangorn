#pragma once

#include <QDomElement>
#include "fangorn/fangornbase.hpp"

struct FangornBTNodeModel {
    BT::NodeType type;
    std::string registrationId;
    BT::PortsList ports;
};

class FangornBTNode : public std::enable_shared_from_this<FangornBTNode> {
    public:
    DEF_SHARED_PTR_TYPES(FangornBTNode);

    FangornBTNode(const FangornBTNode::ConstSharedPtr root, FangornBTNodeModel model, const std::string& name, long id, const std::map<int, FangornBTNode::SharedPtr> children);
    FangornBTNode(const FangornBTNode::ConstSharedPtr root, FangornBTNodeModel model, const std::string& name, long id)
     : FangornBTNode(root, model, name, id, { }) { };
    FangornBTNode(const FangornBTNode::ConstSharedPtr root, const QDomElement& xml, long id);

    //child management
    bool hasChildWithId(long id, long afterId) const;
    bool hasChildWithId(long id) const;
    bool hasChild(const FangornBTNode::ConstSharedPtr child) const;
    long findChildWithName(const std::string& name, long afterId) const;
    long findChildWithName(const std::string& name) const;
    FangornBTNode::SharedPtr getChildById(long id) const;
    FangornBTNode::SharedPtr getChildByName(const std::string& name, long afterId) const;
    FangornBTNode::SharedPtr getChildByName(const std::string& name, const FangornBTNode::ConstSharedPtr afterChild) const;
    FangornBTNode::SharedPtr getChildByName(const std::string& name) const;
    FangornBTNode::SharedPtr eraseChild(long id);
    void addChild(const FangornBTNode::SharedPtr child);
    std::list<long> getChildIds() const;

    //port management
    bool hasPortByName(const std::string& name) const;
    void getPortByName(const std::string& name, BT::PortInfo& port) const;
    void erasePortByName(const std::string& name);
    void addPort(const std::string& portName, const BT::PortInfo& port);
    BT::PortsList getPortsList() const;

    //node management
    const long getId() const;
    const FangornBTNodeModel getNodeModel() const;
    const std::string getName() const;
    const FangornBTNode::ConstSharedPtr getRootNodeContainingThis() const;
    void setName(const std::string& name);
    
    bool hasNodeInSubtree(const FangornBTNode::ConstSharedPtr decendent) const;
    std::vector<FangornBTNode::ConstSharedPtr> getPathToNode(const FangornBTNode::ConstSharedPtr node) const;

    private:
    //unique id allows for easy comparison between other nodes in the tree with the same type and registration id
    const long id; 

    //storing root node allows for easy computation of path and location of other nodes
    const FangornBTNode::ConstSharedPtr root;

    FangornBTNodeModel model;
    std::string name;
    std::map<int, FangornBTNode::SharedPtr> children;
};