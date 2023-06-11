#include "fangorn/fangorncommon.hpp"
#include "fangorn/fangorncomponents.hpp"

//
// Source file for the FangornBTNode class
//

FangornBTNode::FangornBTNode(
    const FangornBTNode::ConstSharedPtr root, 
    FangornBTNodeModel model, 
    const std::string& name, 
    long id, 
    const std::map<int, FangornBTNode::SharedPtr> children) 
: id(id),
  root(root)
{
    this->model = model;
    this->name = name;
    this->children = children;
}


FangornBTNode::FangornBTNode(const FangornBTNode::ConstSharedPtr root, const QDomElement& xml, long id)
: id(id),
  root(root)
{
    FANGORN_ERROR("FangornBTNode from xml is not implemented.");
}


bool FangornBTNode::hasChildWithId(long id, long afterId) const {
    for(auto it = children.begin(); it != children.end(); it++) {
        if(it->first > afterId && it->second->id == id) {
            return true;
        }
    }

    return false;
}


bool FangornBTNode::hasChildWithId(long id) const {
    return hasChildWithId(id, 0);
}


bool FangornBTNode::hasChild(const FangornBTNode::ConstSharedPtr child) const {
    return hasChildWithId(child->id);
}


long FangornBTNode::findChildWithName(const std::string& name, long afterId) const {
    for(auto it = children.begin(); it != children.end(); it++) {
        if(it->first > afterId && it->second->getName() == name) {
            return it->first;
        }
    }

    return -1;
}


long FangornBTNode::findChildWithName(const std::string& name) const {
    return findChildWithName(name, 0);
}


FangornBTNode::SharedPtr FangornBTNode::getChildById(long id) const {
    if(hasChildWithId(id)) {
        return children.at(id);
    }

    std::string nodePath = nodePathToString(getRootNodeContainingThis()->getPathToNode(shared_from_this()));
    FANGORN_WARN("Node %s does not have a child with id %s, but a request was made for one.", nodePath, id);
    return nullptr;
}


FangornBTNode::SharedPtr FangornBTNode::getChildByName(const std::string& name, long afterId) const {
    long id = findChildWithName(name, afterId);
    if(id > -1) {
        return children.at(id);
    }

    std::string nodePath = nodePathToString(getRootNodeContainingThis()->getPathToNode(shared_from_this()));
    FANGORN_WARN("Node %s does not have a child with name %s, after id %d, but a request was made for one.", nodePath, name, afterId);
    return nullptr;
}


FangornBTNode::SharedPtr FangornBTNode::getChildByName(const std::string& name, const FangornBTNode::ConstSharedPtr afterChild) const {
    return getChildByName(name, afterChild->getId());
}


FangornBTNode::SharedPtr FangornBTNode::getChildByName(const std::string& name) const {
    return getChildByName(name, 0);
}


FangornBTNode::SharedPtr FangornBTNode::eraseChild(long id) {
    FangornBTNode::SharedPtr node = getChildById(id);
    if(hasChildWithId(id)) {
        children.erase(id);
    }

    return node;
}


void FangornBTNode::addChild(const FangornBTNode::SharedPtr child) {
    children.insert( {child->getId(), child} );
}


std::list<long> FangornBTNode::getChildIds() const {
    std::list<long> ids;
    for(auto it = children.begin(); it != children.end(); it++) {
        ids.push_back(it->first);
    }

    return ids;
}


bool FangornBTNode::hasPortByName(const std::string& name) const {
    return model.ports.find(name) != model.ports.end();
}


void FangornBTNode::getPortByName(const std::string& name, BT::PortInfo& port) const {
    if(hasPortByName(name)) {
        port = model.ports.at(name);
    }

    std::string nodePath = nodePathToString(getRootNodeContainingThis()->getPathToNode(shared_from_this()));
    FANGORN_WARN("Node %s does not have a port by name %s, but a request was made for one.", nodePath, name);
}


void FangornBTNode::erasePortByName(const std::string& name) {
    if(hasPortByName(name)) {
        model.ports.erase(name);
    } else {
        std::string nodePath = nodePathToString(getRootNodeContainingThis()->getPathToNode(shared_from_this()));
        FANGORN_WARN("Node %s does not have a port by name %s, but a request was made to erase one.", nodePath, name);
    }
}


void FangornBTNode::addPort(const std::string& name, const BT::PortInfo& port) {
    model.ports.insert( {name, port} );
}


BT::PortsList FangornBTNode::getPortsList() const {
    return model.ports;
}


const long FangornBTNode::getId() const {
    return id;
}


const FangornBTNodeModel FangornBTNode::getNodeModel() const {
    return model;
}


const std::string FangornBTNode::getName() const {
    return name;
}


const FangornBTNode::ConstSharedPtr FangornBTNode::getRootNodeContainingThis() const {
    return root;
}


void FangornBTNode::setName(const std::string& name) {
    this->name = name;
}


bool FangornBTNode::hasNodeInSubtree(const FangornBTNode::ConstSharedPtr n) const {
    bool hasNode = n->getId() == getId();
    for(auto it = children.begin(); it != children.end(); it++) {
        if(hasNode) {
            break;
        }

        hasNode = hasNode || getChildById(it->first)->hasNodeInSubtree(n);
    }

    return hasNode;
}


std::vector<FangornBTNode::ConstSharedPtr> FangornBTNode::getPathToNode(const FangornBTNode::ConstSharedPtr node) const {
    for(auto it = children.begin(); it != children.end(); it++) {
        if(it->second->hasNodeInSubtree(node)) {
            std::vector<FangornBTNode::ConstSharedPtr> path = it->second->getPathToNode(node);
            path.insert(path.begin(), shared_from_this());
            return path;
        }
    }

    std::vector<FangornBTNode::ConstSharedPtr> path;
    path.push_back(shared_from_this());
    return path;
}
