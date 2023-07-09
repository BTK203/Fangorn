#include "fangorn/fangorncommon.hpp"
#include "fangorn/model/fangornbtnode.hpp"

//
// Source file for the FangornBTNode class
//


//names are in enum order
static const std::string preconditionNames[4] = {
    "_skipIf",
    "_failureIf",
    "_successIf",
    "_while"
};

//also in enum order
static const std::string postconditionNames[4] = {
    "_onSuccess",
    "_onFailure",
    "_post",
    "_onHalted"
};


unsigned long FangornBtNode::nextAvailableId = 0;

btid_t FangornBtNode::nextAvailableNodeId()
{
    return nextAvailableId++;
}


FangornBtNode::FangornBtNode(
    const FangornBtNode::ConstSharedPtr root, 
    const std::string& registrationId, 
    const std::string& name,
    Attributes attrs, 
    Children children,
    Preconditions preconditions,
    Postconditions postconditions)
 : id(nextAvailableNodeId()),
   root(root),
   registrationId(registrationId),
   attrs(attrs),
   children(children)
{
    setAttributeValue("ID", name);

    for(auto pair : preconditions)
    {
        setPrecondition(pair.first, pair.second);
    }

    for(auto pair : postconditions)
    {
        setPostcondition(pair.first, pair.second);
    }
}


FangornBtNode::FangornBtNode(
    const FangornBtNode::ConstSharedPtr root,
    const std::string& registrationId,
    const std::string& name,
    Attributes attrs,
    Children children)
 : id(nextAvailableNodeId()),
   root(root),
   registrationId(registrationId),
   attrs(attrs),
   children(children)
{
    setAttributeValue("ID", name);
}


FangornBtNode::FangornBtNode(
    const FangornBtNode::ConstSharedPtr root,
    const std::string& registrationId,
    const std::string& name,
    Attributes attrs)
 : id(nextAvailableNodeId()),
   root(root),
   registrationId(registrationId),
   attrs(attrs)
{
    setAttributeValue("ID", name);
}


FangornBtNode::FangornBtNode(
    const FangornBtNode::ConstSharedPtr root,
    const std::string& registrationId,
    const std::string& name)
 : id(nextAvailableNodeId()),
   root(root),
   registrationId(registrationId)
{
    setAttributeValue("ID", name);
}


FangornBtNode::FangornBtNode(const FangornBtNode::ConstSharedPtr root, const QDomElement& xml)
: id(nextAvailableNodeId()),
  root(root),
  registrationId(xml.attribute("ID").isNull() ? xml.nodeValue().toStdString() : xml.attribute("ID").toStdString())
{
    //read in all tag attributes
    QDomNamedNodeMap attributes = xml.attributes();
    for(int i = 0; i < attributes.length(); i++)
    {
        QDomNode n = attributes.item(i);
        setAttributeValue(n.nodeName().toStdString(), n.nodeValue().toStdString());
    }

    if(xml.hasChildNodes()) {
        QDomElement nextChild = xml.firstChildElement();
        while(!nextChild.isNull())
        {
            addChild(std::make_shared<FangornBtNode>(root, nextChild));
            nextChild = nextChild.nextSiblingElement();
        }
    }
}


bool FangornBtNode::hasChildWithId(btid_t id, long afterId) const
{
    for(auto it = children.begin(); it != children.end(); it++)
    {
        if(it->first > afterId && it->second->getId() == id)
        {
            return true;
        }
    }

    return false;
}


bool FangornBtNode::hasChildWithId(btid_t id) const
{
    return hasChildWithId(id, 0);
}


bool FangornBtNode::hasChild(const FangornBtNode::ConstSharedPtr child) const
{
    return hasChildWithId(child->getId());
}


long FangornBtNode::findChildWithName(const std::string& name, long afterId) const
{
    for(auto it = children.begin(); it != children.end(); it++)
    {
        if(it->first > afterId && it->second->getName() == name)
        {
            return it->first;
        }
    }

    return -1;
}


long FangornBtNode::findChildWithName(const std::string& name) const
{
    return findChildWithName(name, 0);
}


FangornBtNode::SharedPtr FangornBtNode::getChildById(btid_t id) const
{
    if(hasChildWithId(id))
    {
        return children.at(id);
    }

    std::string nodePath = nodePathToString(getRootNodeContainingThis()->getPathToNode(shared_from_this()));
    FANGORN_WARN("Node %s does not have a child with id %s, but a request was made for one.", nodePath, id);
    return nullptr;
}


FangornBtNode::SharedPtr FangornBtNode::getChildByName(const std::string& name, long afterId) const
{
    long id = findChildWithName(name, afterId);
    if(id > -1)
    {
        return children.at(id);
    }

    std::string nodePath = nodePathToString(getRootNodeContainingThis()->getPathToNode(shared_from_this()));
    FANGORN_WARN("Node %s does not have a child with name %s, after id %d, but a request was made for one.", nodePath, name, afterId);
    return nullptr;
}


FangornBtNode::SharedPtr FangornBtNode::getChildByName(const std::string& name, const FangornBtNode::ConstSharedPtr afterChild) const
{
    return getChildByName(name, afterChild->getId());
}


FangornBtNode::SharedPtr FangornBtNode::getChildByName(const std::string& name) const
{
    return getChildByName(name, 0);
}


FangornBtNode::SharedPtr FangornBtNode::eraseChild(btid_t id)
{
    FangornBtNode::SharedPtr node = getChildById(id);
    if(hasChildWithId(id))
    {
        children.erase(id);
    }

    return node;
}


void FangornBtNode::addChild(const FangornBtNode::SharedPtr child)
{
    children.insert( {child->getId(), child} );
}


std::list<long> FangornBtNode::getChildIds() const
{
    std::list<long> ids;
    for(auto it = children.begin(); it != children.end(); it++)
    {
        ids.push_back(it->first);
    }

    return ids;
}


bool FangornBtNode::hasAttributeWithName(const std::string& name) const
{
    return attrs.find(name) != attrs.end();
}


std::string FangornBtNode::getAttributeValue(const std::string& name) const
{
    if(hasAttributeWithName(name))
    {
        return attrs.at(name);
    }

    return "";
}


void FangornBtNode::setAttributeValue(const std::string& name, const std::string& value)
{
    attrs[name] = value;
}


std::string FangornBtNode::getPrecondition(FangornPreconditionType type)
{
    return getAttributeValue(preconditionNames[type]);
}


void FangornBtNode::setPrecondition(FangornPreconditionType type, const std::string& script)
{
    setAttributeValue(preconditionNames[type], script);
}


void FangornBtNode::clearPrecondition(FangornPreconditionType type)
{
    setPrecondition(type, "");
}


std::string FangornBtNode::getPostcondition(FangornPostconditionType type)
{
    return getAttributeValue(postconditionNames[type]);
}


void FangornBtNode::setPostcondition(FangornPostconditionType type, const std::string& script)
{
    setAttributeValue(postconditionNames[type], script);
}


void FangornBtNode::clearPostcondition(FangornPostconditionType type)
{
    setPostcondition(type, "");
}


btid_t FangornBtNode::getId() const
{
    return id;
}


std::string FangornBtNode::getName() const
{
    return getAttributeValue("ID");
}


const std::string FangornBtNode::getRegistrationId() const
{
    return registrationId;
}


const FangornBtNode::ConstSharedPtr FangornBtNode::getRootNodeContainingThis() const
{
    return root;
}


void FangornBtNode::setName(const std::string& name)
{
    setAttributeValue("ID", name);
}


bool FangornBtNode::hasNodeInSubtree(const FangornBtNode::ConstSharedPtr n) const
{
    bool hasNode = n->getId() == getId();
    for(auto it = children.begin(); it != children.end(); it++)
    {
        if(hasNode)
        {
            break;
        }

        hasNode = hasNode || getChildById(it->first)->hasNodeInSubtree(n);
    }

    return hasNode;
}


std::vector<FangornBtNode::ConstSharedPtr> FangornBtNode::getPathToNode(const FangornBtNode::ConstSharedPtr node) const
{
    for(auto it = children.begin(); it != children.end(); it++)
    {
        if(it->second->hasNodeInSubtree(node))
        {
            std::vector<FangornBtNode::ConstSharedPtr> path = it->second->getPathToNode(node);
            path.insert(path.begin(), shared_from_this());
            return path;
        }
    }

    std::vector<FangornBtNode::ConstSharedPtr> path;
    path.push_back(shared_from_this());
    return path;
}
