#include <gtest/gtest.h>
#include "fangorntest/fangorntest.hpp"
#include "fangorn/model/fangornbtnode.hpp"
#include "fangorn/util/fangornutils.hpp"
#include "fangorn/util/fangornlogging.hpp"

#define EMPTY_BTNODE(registrationId, name) std::make_shared<FangornBtNode>(registrationId, name)
#define ASSERT_BTNODE_CORRECT(node, rId, name, children, attrs, preconds, postconds) \
    FANGORN_DEBUG("Checking node " #node " for correctness"); checkBtNode(node, rId, name, children, attrs, preconds, postconds)
#define ASSERT_BTNODES_EQ(actual, expected) FANGORN_DEBUG("Comparing experimental node " #actual " to expected node " #expected); checkBtNode(actual, expected)

void checkBtNode(
    FangornBtNode::SharedPtr node,
    const std::string& registrationId,
    const std::string& name,
    FangornBtNode::Children children,
    FangornBtNode::Attributes attrs,
    FangornBtNode::Preconditions preconditions,
    FangornBtNode::Postconditions postconditions) 
{
    ASSERT_EQ(node->getRegistrationId(), registrationId);
    ASSERT_EQ(node->getName(), name);
    
    for(auto childPair : children)
    {
        ASSERT_TRUE(node->hasChildWithId(childPair.first));
        ASSERT_EQ(node->getChildById(childPair.first), childPair.second);
    }

    for(auto attrPair : attrs)
    {
        ASSERT_TRUE(node->hasAttributeWithName(attrPair.first));
        ASSERT_EQ(node->getAttributeValue(attrPair.first), attrPair.second);
    }

    for(auto preconditionPair : preconditions)
    {
        ASSERT_EQ(node->getPrecondition(preconditionPair.first), preconditionPair.second);
    }

    for(auto postconditionPair : postconditions)
    {
        ASSERT_EQ(node->getPostcondition(postconditionPair.first), postconditionPair.second);
    }
}


void checkBtNode(FangornBtNode::SharedPtr actual, FangornBtNode::SharedPtr expected)
{
    FANGORN_DEBUG("Checking behaviortree with rID \"%s\" and name \"%s\"", actual->getRegistrationId().c_str(), actual->getName().c_str());

    //check easy stuff
    ASSERT_EQ(actual->getRegistrationId(), expected->getRegistrationId());
    ASSERT_EQ(actual->getName(), expected->getName());

    //check attrs
    std::list<std::string>
        actualAttrNames = actual->getAttributeNames(),
        expectedAttrNames = expected->getAttributeNames();
        
    ASSERT_EQ(actualAttrNames, expectedAttrNames);

    for(std::list<std::string>::iterator it = actualAttrNames.begin(); it != actualAttrNames.end(); it++)
    {
        ASSERT_EQ(actual->getAttributeValue(*it), expected->getAttributeValue(*it));
    }

    //check children (recursive)
    std::list<btid_t>
        actualChildIds = actual->getChildIds(),
        expectedChildIds = expected->getChildIds();
    
    ASSERT_EQ(actualChildIds.size(), expectedChildIds.size());

    std::list<btid_t>::iterator
        actualKeyIt = actualChildIds.begin(),
        expectedKeyIt = expectedChildIds.begin();
    
    while(actualKeyIt != actualChildIds.end())
    {
        FangornBtNode::SharedPtr
            actualChild = actual->getChildById(*actualKeyIt),
            expectedChild = expected->getChildById(*expectedKeyIt);
        
        ASSERT_BTNODES_EQ(actualChild, expectedChild);

        actualKeyIt++;
        expectedKeyIt++;
    }
}


TEST(FangornBTNode_test, test_FangornBtNode_construct_empty) {
    FangornBtNode::SharedPtr node = EMPTY_BTNODE("some_node", "some_name");
    ASSERT_BTNODE_CORRECT(
        node,
        "some_node",
        "some_name",
        { },
        { },
        { },
        { }
    );
}


TEST(FangornBtNodeTest, test_FangornBtNode_ids)
{
    FangornBtNode::SharedPtr original = EMPTY_BTNODE("a", "a");
    FangornBtNode::SharedPtr test = nullptr;
    for(int i = 1; i <= 25; i++)
    {
        std::string name = std::to_string(i);
        test = EMPTY_BTNODE(name, name);
        ASSERT_EQ(test->getId(), original->getId() + i);
    }
}


TEST(FangornBtNodeTest, test_FangornBtNode_construct_with_ports)
{
    FangornBtNode::Attributes attrs = {
        {"port1", "val1"},
        {"port2", "val2"},
        {"int", "123"},
        {"float", "3.14"}
    };

    FangornBtNode::SharedPtr
        root = EMPTY_BTNODE("root", "root_node"),
        test = std::make_shared<FangornBtNode>(
            "test",
            "test_node",
            attrs
        );
    
    ASSERT_BTNODE_CORRECT(
        test,
        "test",
        "test_node",
        { },
        FangornBtNode::Attributes(attrs),
        { },
        { }
    );
}


TEST(FangornBtNodeTest, test_FangornBtNode_construct_with_everything)
{
    FangornBtNode::SharedPtr
        root = EMPTY_BTNODE("root", "root_node"),
        child1 = EMPTY_BTNODE("a", "a"),
        child2 = EMPTY_BTNODE("b", "b"),
        child3 = EMPTY_BTNODE("c", "c"),
        subchild = EMPTY_BTNODE("d", "d");
    
    child3->addChild(subchild);

    FangornBtNode::Attributes attrs = {
        {"port1", "val1"},
        {"port2", "val2"},
        {"int", "123"},
        {"float", "3.14"}
    };

    FangornBtNode::Children children = {
        {child1->getId(), child1},
        {child2->getId(), child2},
        {child3->getId(), child3}
    };

    FangornBtNode::Preconditions preconds = {
        {FangornPreconditionType::PRECOND_FAIL_IF, "some script"},
        {FangornPreconditionType::PRECOND_SKIP_IF, ""},
        {FangornPreconditionType::PRECOND_SUCCESS_IF, "another script"},
        {FangornPreconditionType::PRECOND_WHILE, "some script"}
    };

    FangornBtNode::Postconditions postconds = {
        {FangornPostconditionType::POSTCOND_ON_FAIL, ""},
        {FangornPostconditionType::POSTCOND_ON_HALTED, ""},
        {FangornPostconditionType::POSTCOND_ON_SUCCESS, "script"},
        {FangornPostconditionType::POSTCOND_POST, "another"}
    };
    
    FangornBtNode::SharedPtr test = std::make_shared<FangornBtNode>(
        "test",
        "test_node",
        attrs,
        children,
        preconds,
        postconds
    );
    
    ASSERT_BTNODE_CORRECT(
        test,
        "test",
        "test_node",
        FangornBtNode::Children(children),
        FangornBtNode::Attributes(attrs),
        FangornBtNode::Preconditions(preconds),
        FangornBtNode::Postconditions(postconds)
    );
}


TEST(FangornBtNodeTest, test_FangornBtNode_construct_small_xml)
{
    const std::string testtree = TESTTREES_LOCATION + "/treeA.xml";

    //create document, locate tree to run
    QDomDocument doc;
    doc.setContent(fileToString(QString::fromStdString(testtree)));
    QDomElement bt = doc.firstChildElement("root").firstChildElement("BehaviorTree").nextSiblingElement("BehaviorTree");

    ASSERT_FALSE(bt.attribute("ID").isNull());
    ASSERT_EQ(bt.attribute("ID").toStdString(), "doc1tree2");

    //perform test
    FangornBtNode::SharedPtr test = std::make_shared<FangornBtNode>(bt);
    
    //assemble tree we want
    FangornBtNode::SharedPtr
        expected = EMPTY_BTNODE("doc1tree2", ""),
        act1     = EMPTY_BTNODE("action1", "");
    
    expected->addChild(act1);

    ASSERT_BTNODES_EQ(test, expected);
}


TEST(FangornBtNodeTest, test_FangornBtNode_construct_with_xml)
{
    const std::string testtree = TESTTREES_LOCATION + "/treeA.xml";

    //create document, locate tree to run
    QDomDocument doc;
    doc.setContent(fileToString(QString::fromStdString(testtree)));
    QDomElement bt = doc.firstChildElement("root").firstChildElement("BehaviorTree");

    ASSERT_FALSE(bt.attribute("ID").isNull());
    ASSERT_EQ(bt.attribute("ID").toStdString(), "doc1tree1");

    //perform test
    FangornBtNode::SharedPtr test = std::make_shared<FangornBtNode>(bt);

    //assemble tree we want
    FangornBtNode::SharedPtr
        root            = EMPTY_BTNODE("doc1tree1", ""),
        sequence        = EMPTY_BTNODE("Sequence", ""),
        act1            = EMPTY_BTNODE("action1", ""),
        sequence2       = EMPTY_BTNODE("Sequence", ""),
        act2            = EMPTY_BTNODE("Action2", "act2");
    
    root->addChild(sequence);
    sequence->addChild(act1);
    sequence->addChild(sequence2);
    sequence2->addChild(act2);
    act2->setAttributeValue("val", "4");

    ASSERT_BTNODES_EQ(test, root);
}


TEST(FangornBtNodeTest, test_FangornBtNode_hasChildWithId_no_children)
{
    FangornBtNode::SharedPtr node = EMPTY_BTNODE("test", "");
    for(btid_t i = node->getId(); i < node->getId() + 50; i++)
    {
        ASSERT_FALSE(node->hasChildWithId(i));
    }
}


TEST(FangornBtNodeTest, test_FangornBtNode_hasChildWithId_with_children)
{
    FangornBtNode::SharedPtr node = EMPTY_BTNODE("test", "");
    btid_t nodeId = node->getId();
    node->addChild(EMPTY_BTNODE("testchild", "child"));
    ASSERT_FALSE(node->hasChildWithId(nodeId));
    ASSERT_TRUE(node->hasChildWithId(nodeId + 1));
    for(btid_t id = nodeId + 2; id < nodeId + 12; id++)
    {
        ASSERT_FALSE(node->hasChildWithId(id));
    }
}


TEST(FangornBtNodeTest, test_FangornBtNode_hasChild_no_children)
{
    FangornBtNode::SharedPtr
        node = EMPTY_BTNODE("test", ""),
        nonChild = EMPTY_BTNODE("nonchild", "nonchild"),
        nonChild2 = EMPTY_BTNODE("nonchild", "nonchild_2");

    ASSERT_FALSE(node->hasChild(node));
    ASSERT_FALSE(node->hasChild(nonChild));
    ASSERT_FALSE(node->hasChild(nonChild2));
}


TEST(FangornBtNodeTest, test_FangornBtNode_hasChild_with_children)
{
    FangornBtNode::SharedPtr
        child1 = EMPTY_BTNODE("child", "child1"),
        child2 = EMPTY_BTNODE("child2", "child2"),
        nonchild1 = EMPTY_BTNODE("nonchild", "nonchild1"),
        nonchild2 = EMPTY_BTNODE("nonchild", "nonchild2");

    FangornBtNode::Attributes attrs = { };
    FangornBtNode::Children children = { 
        { child1->getId(), child1 }, 
        { child2->getId(), child2 } 
    };

    FangornBtNode::SharedPtr node = std::make_shared<FangornBtNode>(
        "test", 
        "", 
        attrs,
        children);
    
    ASSERT_FALSE(node->hasChild(node));
    ASSERT_TRUE(node->hasChild(child1));
    ASSERT_TRUE(node->hasChild(child2));
    ASSERT_FALSE(node->hasChild(nonchild1));
    ASSERT_FALSE(node->hasChild(nonchild2));
}


TEST(FangornBtNodeTest, test_FangornBtNode_findChildHasChildWithName_noChildren)
{
    FangornBtNode::SharedPtr node = EMPTY_BTNODE("test", "test");
    
    //test findChildWithName
    ASSERT_EQ(node->findChildWithName("test"), -1);
    ASSERT_EQ(node->findChildWithName("nonexistent_child"), -1);
    ASSERT_EQ(node->findChildWithName("other_nonexistent"), -1);
    ASSERT_FALSE(node->hasChildWithName("test"));
    ASSERT_FALSE(node->hasChildWithName("nonexistent_child"));
    ASSERT_FALSE(node->hasChildWithName("other_nonexistent"));
}


TEST(FangornBtNodeTest, test_FangornBtNode_findChildHasChildWithName_withChildren)
{
    FangornBtNode::SharedPtr
        child1 = EMPTY_BTNODE("child", "child1"),
        child2 = EMPTY_BTNODE("child2", "child2");

    FangornBtNode::Attributes attrs = { };
    FangornBtNode::Children children = { 
        { child1->getId(), child1 }, 
        { child2->getId(), child2 } 
    };

    FangornBtNode::SharedPtr node = std::make_shared<FangornBtNode>(
        "test", 
        "", 
        attrs,
        children);
    
    ASSERT_EQ(node->findChildWithName("test"), -1);
    ASSERT_EQ(node->findChildWithName("child1"), child1->getId());
    ASSERT_EQ(node->findChildWithName("child1", child1->getId() + 1), -1);
    ASSERT_EQ(node->findChildWithName("child2"), child2->getId());
    ASSERT_EQ(node->findChildWithName("child2", child2->getId() + 1), -1);
    ASSERT_EQ(node->findChildWithName("nonchild1"), -1);
    ASSERT_EQ(node->findChildWithName("nonchild2"), -1);

    ASSERT_FALSE(node->hasChildWithName("test"));
    ASSERT_TRUE(node->hasChildWithName("child1"));
    ASSERT_FALSE(node->hasChildWithName("child1", child1->getId() + 1));
    ASSERT_TRUE(node->hasChildWithName("child2"));
    ASSERT_FALSE(node->hasChildWithName("child2", child2->getId() + 1));
    ASSERT_FALSE(node->hasChildWithName("nonchild1"));
    ASSERT_FALSE(node->hasChildWithName("nonchild2"));
}


TEST(FangornBtNodeTest, test_FangornBtNode_getChildById_noChildren)
{
    FangornBtNode::SharedPtr node = EMPTY_BTNODE("node", "");

    ASSERT_THROW(node->getChildById(node->getId()), std::invalid_argument);
    ASSERT_THROW(node->getChildById(12345), std::invalid_argument);
}


TEST(FangornBtNodeTest, test_FangornBtNode_getChildById_withChildren)
{
    FangornBtNode::SharedPtr
        child1 = EMPTY_BTNODE("child", "child1"),
        child2 = EMPTY_BTNODE("child2", "child2"),
        nonchild = EMPTY_BTNODE("nonchild", "nc");

    FangornBtNode::Attributes attrs = { };
    FangornBtNode::Children children = { 
        { child1->getId(), child1 }, 
        { child2->getId(), child2 } 
    };

    FangornBtNode::SharedPtr node = std::make_shared<FangornBtNode>(
        "test", 
        "", 
        attrs,
        children);

    FangornBtNode::SharedPtr
        foundChild1,
        foundChild2;
    
    ASSERT_THROW(node->getChildById(node->getId()), std::invalid_argument);
    ASSERT_THROW(node->getChildById(nonchild->getId()), std::invalid_argument);
    ASSERT_NO_THROW(foundChild1 = node->getChildById(child1->getId()));
    ASSERT_NO_THROW(foundChild2 = node->getChildById(child2->getId()));

    ASSERT_BTNODES_EQ(foundChild1, child1);
    ASSERT_BTNODES_EQ(foundChild2, child2);
}


TEST(FangornBtNodeTest, test_FangornBtNode_getChildByName_noChildren)
{
    FangornBtNode::SharedPtr node = EMPTY_BTNODE("test", "test_node");
    ASSERT_THROW(node->getChildByName(node->getName()), std::invalid_argument);
    ASSERT_THROW(node->getChildByName("nonexistent_1"), std::invalid_argument);
    ASSERT_THROW(node->getChildByName("nonexistent_2"), std::invalid_argument);
}


TEST(FangornBtNodeTest, test_FangornBtNode_getChildByName_withChildren)
{
    FangornBtNode::SharedPtr
        child1 = EMPTY_BTNODE("child", "child1"),
        child2 = EMPTY_BTNODE("child2", "child2"),
        nonchild = EMPTY_BTNODE("nonchild", "nc");

    FangornBtNode::Attributes attrs = { };
    FangornBtNode::Children children = { 
        { child1->getId(), child1 }, 
        { child2->getId(), child2 } 
    };

    FangornBtNode::SharedPtr node = std::make_shared<FangornBtNode>(
        "test", 
        "", 
        attrs,
        children);

    FangornBtNode::SharedPtr
        foundChild1,
        foundChild2,
        foundChild3;
    
    ASSERT_THROW(node->getChildByName(node->getName()), std::invalid_argument);
    ASSERT_NO_THROW(foundChild1 = node->getChildByName("child1"));
    ASSERT_NO_THROW(foundChild2 = node->getChildByName(child2->getName()));
    ASSERT_BTNODES_EQ(foundChild1, child1);
    ASSERT_BTNODES_EQ(foundChild2, child2);
    ASSERT_THROW(node->getChildByName("nonexistent_child"), std::invalid_argument);
    ASSERT_THROW(node->getChildByName("another_nonexistent_child"), std::invalid_argument);
    //test find after child (checking getChildByName with afterid)
    ASSERT_NO_THROW(foundChild3 = node->getChildByName("child2", foundChild1));
    ASSERT_BTNODES_EQ(foundChild3, child2);
    ASSERT_THROW(node->getChildByName("child1", child2), std::invalid_argument);
}


TEST(FangornBtNodeTest, test_FangornBtNode_eraseChild_noChildren)
{
    FangornBtNode::SharedPtr 
        node = EMPTY_BTNODE("test", "test_node"),
        nonchild = EMPTY_BTNODE("notachild", "");

    ASSERT_THROW(node->eraseChild(node->getId()), std::invalid_argument);
    ASSERT_THROW(node->eraseChild(node), std::invalid_argument);
    ASSERT_THROW(node->eraseChild(nonchild), std::invalid_argument);
}


TEST(FangornBtNodeTest, test_FangornBtNode_eraseChild_withChildren)
{
    FangornBtNode::SharedPtr
        child1 = EMPTY_BTNODE("child", "child1"),
        child2 = EMPTY_BTNODE("child2", "child2"),
        nonchild = EMPTY_BTNODE("nonchild", "nc");

    FangornBtNode::Attributes attrs = { };
    FangornBtNode::Children children = { 
        { child1->getId(), child1 }, 
        { child2->getId(), child2 } 
    };

    FangornBtNode::SharedPtr node = std::make_shared<FangornBtNode>(
        "test", 
        "", 
        attrs,
        children);

    FangornBtNode::SharedPtr
        foundChild1,
        foundChild2;
        
    ASSERT_THROW(node->eraseChild(node), std::invalid_argument);
    ASSERT_THROW(node->eraseChild(nonchild), std::invalid_argument);
    ASSERT_TRUE(node->hasChild(child1));
    ASSERT_TRUE(node->hasChild(child2));
    ASSERT_NO_THROW(foundChild1 = node->eraseChild(child1));
    ASSERT_BTNODES_EQ(foundChild1, child1);
    ASSERT_FALSE(node->hasChild(child1));
    ASSERT_TRUE(node->hasChild(child2));
    ASSERT_NO_THROW(foundChild2 = node->eraseChild(child2));
    ASSERT_FALSE(node->hasChild(child1));
    ASSERT_FALSE(node->hasChild(child2));
}


TEST(FangornBtNodeTest, test_FangornBtNode_addChild)
{
    FangornBtNode::SharedPtr 
        node = EMPTY_BTNODE("node", "n"),
        child1 = EMPTY_BTNODE("child1", "c1"),
        child2 = EMPTY_BTNODE("child2", "c2");

    ASSERT_FALSE(node->hasChild(child1));
    ASSERT_FALSE(node->hasChild(child2));
    node->addChild(child1);
    ASSERT_TRUE(node->hasChild(child1));
    ASSERT_FALSE(node->hasChild(child2));
    node->addChild(child2);
    ASSERT_TRUE(node->hasChild(child1));
    ASSERT_TRUE(node->hasChild(child2));
    
    ASSERT_BTNODES_EQ(node->getChildById(child1->getId()), child1);
    ASSERT_BTNODES_EQ(node->getChildById(child2->getId()), child2);
}


TEST(FangornBtNodeTest, test_FangornBtNode_getChildIds)
{
    FangornBtNode::SharedPtr node = EMPTY_BTNODE("node", "");
    std::list<btid_t> expected;
    ASSERT_EQ(node->getChildIds(), expected);
    for(int i = 0; i < 25; i++)
    {
        FangornBtNode::SharedPtr child = EMPTY_BTNODE("child", "");
        node->addChild(child);
        expected.push_back(child->getId());
        ASSERT_EQ(node->getChildIds(), expected);
    }
}


TEST(FangornBtNodeTest, test_FangornBtNode_getAttributeNames_noExtras)
{
    FangornBtNode::SharedPtr 
        justRId = EMPTY_BTNODE("test", ""),
        rIdAndName = EMPTY_BTNODE("rid", "name");

    std::list<std::string> 
        expectedJustRId = { "ID" },
        expectedRIdAndName = { "ID", "name" };

    ASSERT_EQ(justRId->getAttributeNames(), expectedJustRId);
    ASSERT_EQ(rIdAndName->getAttributeNames(), expectedRIdAndName);
}


TEST(FangornBtNodeTest, test_FangornBtNode_getAttributeNames_withAttrs)
{
    FangornBtNode::Attributes attrs = {
        { "attr1", "val1" },
        { "attr2", "val2" },
        { "attr3", "val3" }
    };

    FangornBtNode::SharedPtr node = std::make_shared<FangornBtNode>(
        "node",
        "some_name",
        attrs
    );

    std::list<std::string> expectedAttrs = {
        "ID",
        "name",
        "attr1",
        "attr2",
        "attr3"
    };

    ASSERT_EQ(node->getAttributeNames(), expectedAttrs);
}


TEST(FangornBtNodeTest, test_FangornBtNode_setAttributeValue_getAttributeValue)
{
    FangornBtNode::SharedPtr node = EMPTY_BTNODE("node", "");
    std::list<std::string> expectedNames = { "ID" };
    ASSERT_EQ(node->getAttributeNames(), expectedNames);
    for(int i = 0; i < 25; i++)
    {
        std::string 
            newName = "attr" + std::to_string(i),
            newvalue = "val" + std::to_string(i);

        node->setAttributeValue(newName, "some_value");
        expectedNames.push_back(newName);
        ASSERT_EQ(node->getAttributeNames(), expectedNames);
        ASSERT_EQ(node->getAttributeValue(newName), newvalue);
        ASSERT_EQ(node->getAttributeValue("ID"), "node");
    }
}
