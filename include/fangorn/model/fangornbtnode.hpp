#pragma once

#include <QDomElement>
#include "fangorn/fangornbase.hpp"

typedef unsigned long btid_t;

enum FangornPreconditionType {
    PRECOND_SKIP_IF,
    PRECOND_FAIL_IF,
    PRECOND_SUCCESS_IF,
    PRECOND_WHILE
};

enum FangornPostconditionType {
    POSTCOND_ON_SUCCESS,
    POSTCOND_ON_FAIL,
    POSTCOND_POST,
    POSTCOND_ON_HALTED
};


class FangornBtNode : public std::enable_shared_from_this<FangornBtNode> {
    public:
    DEF_SMART_PTR_TYPES(FangornBtNode);
    typedef std::map<btid_t, FangornBtNode::SharedPtr> Children;
    typedef std::map<const std::string, std::string> Attributes;
    typedef std::map<FangornPreconditionType, std::string> Preconditions;
    typedef std::map<FangornPostconditionType, std::string> Postconditions;

    FangornBtNode(
        const std::string& registrationId, 
        const std::string& name = "",
        Attributes attrs = { },
        Children children = { },
        Preconditions preconditions = { },
        Postconditions postconditions = { });

    FangornBtNode(const QDomElement& xml);

    //child management
    bool hasChildWithId(btid_t id, btid_t afterId = 0) const;
    bool hasChild(const FangornBtNode::ConstSharedPtr child) const;
    long findChildWithName(const std::string& name, btid_t afterId = 0) const;
    bool hasChildWithName(const std::string& name, btid_t afterId = 0) const;
    FangornBtNode::SharedPtr getChildById(btid_t id) const;
    FangornBtNode::SharedPtr getChildByName(const std::string& name, btid_t afterId = 0) const;
    FangornBtNode::SharedPtr getChildByName(const std::string& name, const FangornBtNode::ConstSharedPtr afterChild) const;
    FangornBtNode::SharedPtr eraseChild(btid_t id);
    FangornBtNode::SharedPtr eraseChild(FangornBtNode::ConstSharedPtr child);
    void addChild(const FangornBtNode::SharedPtr child);
    std::list<btid_t> getChildIds() const;
    //port value management
    std::list<std::string> getAttributeNames() const;
    void setAttributeValue(const std::string& name, const std::string& value);
    bool hasAttributeWithName(const std::string& name) const;
    std::string getAttributeValue(const std::string& name) const;

    //precondition management
    std::string getPrecondition(FangornPreconditionType type);
    void setPrecondition(FangornPreconditionType type, const std::string& script);
    void clearPrecondition(FangornPreconditionType type);

    //postcondition management
    std::string getPostcondition(FangornPostconditionType type);
    void setPostcondition(FangornPostconditionType type, const std::string& script);
    void clearPostcondition(FangornPostconditionType type);

    //node management
    const btid_t getId() const;
    std::string getName() const;
    const std::string getRegistrationId() const;
    void setName(const std::string& name);
    void setRegistrationId(const std::string& id);

    bool hasNodeInSubtree(const FangornBtNode::ConstSharedPtr decendent) const;

    private:
    static const btid_t nextAvailableNodeId();
    static btid_t nextAvailableId;
   
    //unique id allows for easy comparison between other nodes in the tree with the same type and registration id
    const btid_t id; 

    Children children;
    Attributes attrs;
};
