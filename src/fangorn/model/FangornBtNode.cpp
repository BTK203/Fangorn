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

const btid_t FangornBtNode::nextAvailableNodeId()
{
    return nextAvailableId++;
}


FangornBtNode::FangornBtNode (
    const std::string& registrationId, 
    const std::string& name,
    Attributes attrs,
    Children children,
    Preconditions preconditions,
    Postconditions postconditions)
 : id(nextAvailableNodeId()),
   attrs(attrs),
   children(children)
{
    setRegistrationId(registrationId);
    setName(name);

    for(auto pair : preconditions)
    {
        setPrecondition(pair.first, pair.second);
    }

    for(auto pair : postconditions)
    {
        setPostcondition(pair.first, pair.second);
    }
}


FangornBtNode::FangornBtNode(const QDomElement& xml)
: id(nextAvailableNodeId())
{
    setRegistrationId(xml.attribute("ID").isNull() ? xml.nodeName().toStdString() : xml.attribute("ID").toStdString());

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
            addChild(std::make_shared<FangornBtNode>(nextChild));
            nextChild = nextChild.nextSiblingElement();
        }
    }
}


bool FangornBtNode::hasChildWithId(btid_t id, btid_t afterId) const
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


bool FangornBtNode::hasChild(const FangornBtNode::ConstSharedPtr child) const
{
    return hasChildWithId(child->getId());
}


long FangornBtNode::findChildWithName(const std::string& name, btid_t afterId) const
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


bool FangornBtNode::hasChildWithName(const std::string& name, btid_t afterId) const
{
    return findChildWithName(name, afterId) > -1;
}


FangornBtNode::SharedPtr FangornBtNode::getChildById(btid_t id) const
{
    if(hasChildWithId(id))
    {
        return children.at(id);
    }

    throw std::invalid_argument(std::to_string(getId()) + " has no child with id " + std::to_string(id));
}


FangornBtNode::SharedPtr FangornBtNode::getChildByName(const std::string& name, btid_t afterId) const
{
    long id = findChildWithName(name, afterId);
    if(id > -1)
    {
        return children.at(id);
    }

    throw std::invalid_argument(std::to_string(getId()) + " has no child with name " + name + " after id " + std::to_string(afterId));
}


FangornBtNode::SharedPtr FangornBtNode::getChildByName(const std::string& name, const FangornBtNode::ConstSharedPtr afterChild) const
{
    return getChildByName(name, afterChild->getId());
}


FangornBtNode::SharedPtr FangornBtNode::eraseChild(btid_t id)
{
    FangornBtNode::SharedPtr node = getChildById(id); //will throw if id is not a child
    children.erase(id);
    return node;
}


FangornBtNode::SharedPtr FangornBtNode::eraseChild(FangornBtNode::ConstSharedPtr child)
{
    return eraseChild(child->getId());
}


void FangornBtNode::addChild(const FangornBtNode::SharedPtr child)
{
    children.insert( {child->getId(), child} );
}


std::list<btid_t> FangornBtNode::getChildIds() const
{
    std::list<btid_t> ids;
    for(auto it = children.begin(); it != children.end(); it++)
    {
        ids.push_back(it->first);
    }

    return ids;
}


std::list<std::string> FangornBtNode::getAttributeNames() const
{
    std::list<std::string> names;
    for(auto pair : attrs)
    {
        if(pair.second.size() > 0)
        {
            names.push_back(pair.first);
        }
    }

    return names;
}


void FangornBtNode::setAttributeValue(const std::string& name, const std::string& value)
{
    attrs[name] = value;
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


const btid_t FangornBtNode::getId() const
{
    return id;
}


std::string FangornBtNode::getName() const
{
    return getAttributeValue("name");
}


const std::string FangornBtNode::getRegistrationId() const
{
    return getAttributeValue("ID");
}


void FangornBtNode::setName(const std::string& name)
{
    setAttributeValue("name", name);
}


void FangornBtNode::setRegistrationId(const std::string& id)
{
    setAttributeValue("ID", id);
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
