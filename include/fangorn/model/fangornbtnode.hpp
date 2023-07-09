#pragma once

#include <QDomElement>
#include "fangorn/fangornbase.hpp"

typedef const unsigned long btid_t;

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
    DEF_SHARED_PTR_TYPES(FangornBtNode);
    typedef std::map<btid_t, FangornBtNode::SharedPtr> Children;
    typedef std::map<const std::string, std::string> Attributes;
    typedef std::map<FangornPreconditionType, std::string> Preconditions;
    typedef std::map<FangornPostconditionType, std::string> Postconditions;

    FangornBtNode(
        const FangornBtNode::ConstSharedPtr root, 
        const std::string& registrationId, 
        const std::string& name,
        Attributes attrs, 
        Children children,
        Preconditions preconditions,
        Postconditions postconditions);
    
    FangornBtNode(
        const FangornBtNode::ConstSharedPtr root,
        const std::string& registrationId,
        const std::string& name,
        Attributes attrs,
        Children children);
    
    FangornBtNode(
        const FangornBtNode::ConstSharedPtr root,
        const std::string& registrationId,
        const std::string& name,
        Attributes attrs);

    FangornBtNode(
        const FangornBtNode::ConstSharedPtr root,
        const std::string& registrationId,
        const std::string& name);

    FangornBtNode(const FangornBtNode::ConstSharedPtr root, const QDomElement& xml);
    FangornBtNode(const QDomElement& xml)
     : FangornBtNode(shared_from_this(), xml) { };

    //child management
    bool hasChildWithId(btid_t id, long afterId) const;
    bool hasChildWithId(btid_t id) const;
    bool hasChild(const FangornBtNode::ConstSharedPtr child) const;
    long findChildWithName(const std::string& name, long afterId) const;
    long findChildWithName(const std::string& name) const;
    FangornBtNode::SharedPtr getChildById(btid_t id) const;
    FangornBtNode::SharedPtr getChildByName(const std::string& name, long afterId) const;
    FangornBtNode::SharedPtr getChildByName(const std::string& name, const FangornBtNode::ConstSharedPtr afterChild) const;
    FangornBtNode::SharedPtr getChildByName(const std::string& name) const;
    FangornBtNode::SharedPtr eraseChild(btid_t id);
    void addChild(const FangornBtNode::SharedPtr child);
    std::list<long> getChildIds() const;

    //port value management
    bool hasAttributeWithName(const std::string& name) const;
    std::string getAttributeValue(const std::string& name) const;
    void setAttributeValue(const std::string& name, const std::string& value);

    //precondition management
    std::string getPrecondition(FangornPreconditionType type);
    void setPrecondition(FangornPreconditionType type, const std::string& script);
    void clearPrecondition(FangornPreconditionType type);

    //postcondition management
    std::string getPostcondition(FangornPostconditionType type);
    void setPostcondition(FangornPostconditionType type, const std::string& script);
    void clearPostcondition(FangornPostconditionType type);

    //node management
    btid_t getId() const;
    std::string getName() const;
    const std::string getRegistrationId() const;
    const FangornBtNode::ConstSharedPtr getRootNodeContainingThis() const;
    void setName(const std::string& name);
    
    bool hasNodeInSubtree(const FangornBtNode::ConstSharedPtr decendent) const;
    std::vector<FangornBtNode::ConstSharedPtr> getPathToNode(const FangornBtNode::ConstSharedPtr node) const;

    private:
    static btid_t nextAvailableNodeId();
    static unsigned long nextAvailableId;
   
    //unique id allows for easy comparison between other nodes in the tree with the same type and registration id
    btid_t id; 
    const std::string registrationId;

    //storing root node allows for easy computation of path and location of other nodes
    const FangornBtNode::ConstSharedPtr root;

    Children children;
    Attributes attrs;
};
