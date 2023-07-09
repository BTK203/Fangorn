#include <gtest/gtest.h>
#include "fangorntest/fangorntest.hpp"
#include "fangorn/model/fangornbtnode.hpp"

#define EMPTY_BTNODE_NULL_ROOT(registrationId, name) std::make_shared<FangornBtNode>(nullptr, registrationId, name)
#define EMPTY_BTNODE(root, registrationId, name) std::make_shared<FangornBtNode>(root, registrationId, name)

void checkBtNode(
    FangornBtNode::SharedPtr node,
    const std::string& registrationId,
    const FangornBtNode::ConstSharedPtr root,
    const std::string& name,
    FangornBtNode::Children children,
    FangornBtNode::Attributes attrs,
    FangornBtNode::Preconditions preconditions,
    FangornBtNode::Postconditions postconditions) 
{
    ASSERT_EQ(node->getRegistrationId(), registrationId);
    ASSERT_EQ(node->getRootNodeContainingThis(), root);
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


TEST(FangornBTNode_test, test_FangornBtNode_construct_empty_with_null_root) {
    FangornBtNode::SharedPtr node = EMPTY_BTNODE_NULL_ROOT("some_node", "some_name");
    checkBtNode(
        node,
        "some_node",
        nullptr,
        "some_name",
        { },
        { },
        { },
        { }
    );
}


TEST(FangornBtNodeTest, test_FangornBtNode_ids)
{
    FangornBtNode::SharedPtr original = EMPTY_BTNODE_NULL_ROOT("a", "a");
    FangornBtNode::SharedPtr test = nullptr;
    for(int i = 1; i <= 25; i++)
    {
        std::string name = std::to_string(i);
        test = EMPTY_BTNODE_NULL_ROOT(name, name);
        ASSERT_EQ(test->getId(), original->getId() + i);
    }
}


TEST(FangornBtNodeTest, test_FangornBtNode_construct_empty_with_empty_root)
{
    FangornBtNode::SharedPtr
        root = EMPTY_BTNODE_NULL_ROOT("root", "root_node"),
        test = EMPTY_BTNODE(root, "test", "test_node");
    
    checkBtNode(
        test,
        "test",
        root,
        "test_node",
        { },
        { },
        { },
        { }
    );
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
        root = EMPTY_BTNODE_NULL_ROOT("root", "root_node"),
        test = std::make_shared<FangornBtNode>(
            root,
            "test",
            "test_node",
            attrs
        );
    
    checkBtNode(
        test,
        "test",
        root,
        "test_node",
        { },
        FangornBtNode::Attributes(attrs),
        { },
        { }
    );
}


TEST(FangornBtNodeTest, test_FangornBtNode_construct_with_children)
{
    FangornBtNode::SharedPtr
        root = EMPTY_BTNODE_NULL_ROOT("root", "root_node"),
        child1 = EMPTY_BTNODE(root, "a", "a"),
        child2 = EMPTY_BTNODE(root, "b", "b"),
        child3 = EMPTY_BTNODE(root, "c", "c"),
        subchild = EMPTY_BTNODE(root, "d", "d");
    
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
    
    FangornBtNode::SharedPtr test = std::make_shared<FangornBtNode>(
        root,
        "test",
        "test_node",
        attrs,
        children
    );
    
    checkBtNode(
        test,
        "test",
        root,
        "test_node",
        FangornBtNode::Children(children),
        FangornBtNode::Attributes(attrs),
        { },
        { }
    );
}


TEST(FangornBtNodeTest, test_FangornBtNode_construct_with_conditions)
{
    FangornBtNode::SharedPtr
        root = EMPTY_BTNODE_NULL_ROOT("root", "root_node"),
        child1 = EMPTY_BTNODE(root, "a", "a"),
        child2 = EMPTY_BTNODE(root, "b", "b"),
        child3 = EMPTY_BTNODE(root, "c", "c"),
        subchild = EMPTY_BTNODE(root, "d", "d");
    
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
        root,
        "test",
        "test_node",
        attrs,
        children,
        preconds,
        postconds
    );
    
    checkBtNode(
        test,
        "test",
        root,
        "test_node",
        FangornBtNode::Children(children),
        FangornBtNode::Attributes(attrs),
        FangornBtNode::Preconditions(preconds),
        FangornBtNode::Postconditions(postconds)
    );
}

TEST(FangornBtNodeTest, test_FangornBtNode_construct_with_xml_no_root)
{
    const std::string testtree = TESTTREES_LOCATION + "/treeA.xml";

    //create document, locate tree to run
    QDomDocument doc;
    //TODO: populate document with setcontent
    QDomElement bt = doc.firstChildElement("root").firstChildElement("BehaviorTree");
    std::cout << "asdf: " << bt.isNull() << std::endl;

    ASSERT_EQ(bt.attribute("ID").toStdString(), "doc1tree1");

    //perform test
    FangornBtNode::SharedPtr test = std::make_shared<FangornBtNode>(bt);
    
    //assemble tree we want
    FangornBtNode::SharedPtr
        root            = EMPTY_BTNODE(root, "BehaviorTree", "doc1tree1"),
        sequence        = EMPTY_BTNODE(root, "Sequence", ""),
        act1            = EMPTY_BTNODE(root, "action1", ""),
        sequence2       = EMPTY_BTNODE(root, "Sequence", ""),
        act2            = EMPTY_BTNODE(root, "Action2", "act2");
    
    root->addChild(sequence);
    sequence->addChild(act1);
    sequence->addChild(sequence2);
    sequence2->addChild(act2);
    act2->setAttributeValue("val", "4");

    FangornBtNode::Children children = { 
        {sequence->getId(), sequence}
    };

    checkBtNode(
        test,
        "BehaviorTree",
        root,
        "doc1tree1",
        children,
        { },
        { },
        { }
    );
}
