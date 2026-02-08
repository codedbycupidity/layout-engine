#include <gtest/gtest.h>
#include "core/LayoutNode.h"

TEST(LayoutNodeTest, Construction){
    LayoutNode node("test-node");

    EXPECT_EQ(node.getId(), "test-node");

    EXPECT_EQ(node.getDisplay(), Display::Block);
    EXPECT_FALSE(node.hasExplicitWidth());   // width should be auto
    EXPECT_FALSE(node.hasExplicitHeight());  // height should be auto


    // Verify edges default to zero
    EXPECT_FLOAT_EQ(node.getMargin().horizontal(), 0.0f);
    EXPECT_FLOAT_EQ(node.getPadding().vertical(), 0.0f);
    EXPECT_FLOAT_EQ(node.getBorder().horizontal(), 0.0f);

    // Verify flex defaults
    EXPECT_EQ(node.getFlexDirection(), FlexDirection::Row);
    EXPECT_EQ(node.getJustifyContent(), JustifyContent::FlexStart);
    EXPECT_EQ(node.getAlignItems(), AlignItems::Stretch);

    // Verify computed values start at zero
    EXPECT_FLOAT_EQ(node.getComputedX(), 0.0f);
    EXPECT_FLOAT_EQ(node.getComputedY(), 0.0f);
    EXPECT_FLOAT_EQ(node.getComputedWidth(), 0.0f);
    EXPECT_FLOAT_EQ(node.getComputedHeight(), 0.0f);

    // Verify tree structure
    EXPECT_EQ(node.getParent(), nullptr);  // No parent
    EXPECT_EQ(node.getChildCount(), 0);    // No children

}

TEST(LayoutNodeTest, SetProperties){
    LayoutNode node("styled-node");

    // Set display
    node.setDisplay(Display::Flex);
    EXPECT_EQ(node.getDisplay(), Display::Flex);

    // Set dimensions
    node.setWidth(300.0f);
    node.setHeight(200.0f);
    EXPECT_TRUE(node.hasExplicitWidth());
    EXPECT_TRUE(node.hasExplicitHeight());
    EXPECT_FLOAT_EQ(node.getWidth().value(), 300.0f);
    EXPECT_FLOAT_EQ(node.getHeight().value(), 200.0f);

    // Set edge values
    EdgeValues margin(10.0f);
    EdgeValues padding(15.0f, 20.0f, 15.0f, 20.0f);
    EdgeValues border(2.0f);

    node.setMargin(margin);
    node.setPadding(padding);
    node.setBorder(border);

    EXPECT_FLOAT_EQ(node.getMargin().top(), 10.0f);
    EXPECT_FLOAT_EQ(node.getPadding().horizontal(), 40.0f);  // 20 + 20
    EXPECT_FLOAT_EQ(node.getBorder().vertical(), 4.0f);      // 2 + 2

    // Set flex properties
    node.setFlexDirection(FlexDirection::Column);
    node.setJustifyContent(JustifyContent::Center);
    node.setAlignItems(AlignItems::FlexEnd);

    EXPECT_EQ(node.getFlexDirection(), FlexDirection::Column);
    EXPECT_EQ(node.getJustifyContent(), JustifyContent::Center);
    EXPECT_EQ(node.getAlignItems(), AlignItems::FlexEnd);
}

TEST(LayoutNodeTest, ComputedValues) {
    LayoutNode node("computed-node");

    // Initially zero
    EXPECT_FLOAT_EQ(node.getComputedX(), 0.0f);
    EXPECT_FLOAT_EQ(node.getComputedY(), 0.0f);
    EXPECT_FLOAT_EQ(node.getComputedWidth(), 0.0f);
    EXPECT_FLOAT_EQ(node.getComputedHeight(), 0.0f);

    // Set computed position
    node.setComputedPosition(50.0f, 100.0f);
    EXPECT_FLOAT_EQ(node.getComputedX(), 50.0f);
    EXPECT_FLOAT_EQ(node.getComputedY(), 100.0f);

    // Set computed size
    node.setComputedSize(400.0f, 300.0f);
    EXPECT_FLOAT_EQ(node.getComputedWidth(), 400.0f);
    EXPECT_FLOAT_EQ(node.getComputedHeight(), 300.0f);
}


TEST(LayoutNodeTest, AddChild) {
    auto parent = std::make_unique<LayoutNode>("parent");
    auto child1 = std::make_unique<LayoutNode>("child1");
    auto child2 = std::make_unique<LayoutNode>("child2");

    // Store raw pointers before moving
    LayoutNode* child1_ptr = child1.get();
    LayoutNode* child2_ptr = child2.get();

    // Add children
    parent->addChild(std::move(child1));
    parent->addChild(std::move(child2));

    // Verify child count
    EXPECT_EQ(parent->getChildCount(), 2);

    // Verify parent pointers set correctly
    EXPECT_EQ(child1_ptr->getParent(), parent.get());
    EXPECT_EQ(child2_ptr->getParent(), child2_ptr->getParent());

    // Verify we can access children
    EXPECT_EQ(parent->getChild(0).getId(), "child1");
    EXPECT_EQ(parent->getChild(1).getId(), "child2");
}


TEST(LayoutNodeTest, ParentChildRelationships) {
    auto root = std::make_unique<LayoutNode>("root");
    auto child = std::make_unique<LayoutNode>("child");
    auto grandchild = std::make_unique<LayoutNode>("grandchild");

    // Build tree: root -> child -> grandchild
    LayoutNode* child_ptr = child.get();
    LayoutNode* grandchild_ptr = grandchild.get();

    child->addChild(std::move(grandchild));
    root->addChild(std::move(child));

    // Verify relationships
    EXPECT_EQ(root->getParent(), nullptr);           // Root has no parent
    EXPECT_EQ(child_ptr->getParent(), root.get());   // Child's parent is root
    EXPECT_EQ(grandchild_ptr->getParent(), child_ptr); // Grandchild's parent is child

    // Verify tree structure
    EXPECT_EQ(root->getChildCount(), 1);
    EXPECT_EQ(child_ptr->getChildCount(), 1);
    EXPECT_EQ(grandchild_ptr->getChildCount(), 0);
}


TEST(LayoutNodeTest, HelperMethods) {
    LayoutNode node("helper-node");

    // Set edge values
    EdgeValues margin(10.0f);        // 10px all sides
    EdgeValues padding(5.0f, 15.0f, 5.0f, 15.0f);  // 5px top/bottom, 15px left/right
    EdgeValues border(2.0f);         // 2px all sides

    node.setMargin(margin);
    node.setPadding(padding);
    node.setBorder(border);

    // Test total horizontal space
    // margin: 10 + 10 = 20
    // padding: 15 + 15 = 30
    // border: 2 + 2 = 4
    // Total: 54px
    EXPECT_FLOAT_EQ(node.getTotalHorizontalSpace(), 54.0f);

    // Test total vertical space
    // margin: 10 + 10 = 20
    // padding: 5 + 5 = 10
    // border: 2 + 2 = 4
    // Total: 34px
    EXPECT_FLOAT_EQ(node.getTotalVerticalSpace(), 34.0f);
}


TEST(LayoutNodeTest, BoundsChecking) {
    LayoutNode node("bounds-node");

    // Should throw when accessing non-existent child
    EXPECT_THROW(node.getChild(0), std::out_of_range);
    EXPECT_THROW(node.getChild(100), std::out_of_range);

    // Add one child
    auto child = std::make_unique<LayoutNode>("child");
    node.addChild(std::move(child));

    // Index 0 should work
    EXPECT_NO_THROW(node.getChild(0));

    // Index 1 should throw
    EXPECT_THROW(node.getChild(1), std::out_of_range);
}
