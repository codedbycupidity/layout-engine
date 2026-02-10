#include <gtest/gtest.h>
#include "layout/BlockLayout.h"
#include "core/LayoutNode.h"
#include "core/layout/LayoutConstraints.h"
#include <memory>

// Test 1: Single child - verify position and container height
TEST(BlockLayoutTest, SingleChild) {
    auto parent = std::make_unique<LayoutNode>("parent");
    parent->setDisplay(Display::Block);

    auto child = std::make_unique<LayoutNode>("child");
    child->setDisplay(Display::Block);
    child->setHeight(100.0f);

    parent->addChild(std::move(child));

    LayoutConstraints constraints(800.0f, 600.0f);
    BlockLayout::layout(*parent, constraints);

    EXPECT_FLOAT_EQ(parent->getComputedWidth(), 800.0f);
    EXPECT_FLOAT_EQ(parent->getComputedHeight(), 100.0f);

    const LayoutNode& child_ref = parent->getChild(0);
    EXPECT_FLOAT_EQ(child_ref.getComputedX(), 0.0f);
    EXPECT_FLOAT_EQ(child_ref.getComputedY(), 0.0f);
    EXPECT_FLOAT_EQ(child_ref.getComputedWidth(), 800.0f);
    EXPECT_FLOAT_EQ(child_ref.getComputedHeight(), 100.0f);
}

// Test 2: Multiple children - verify vertical stacking
TEST(BlockLayoutTest, MultipleChildren) {
    auto parent = std::make_unique<LayoutNode>("parent");
    parent->setDisplay(Display::Block);

    auto child1 = std::make_unique<LayoutNode>("child1");
    child1->setDisplay(Display::Block);
    child1->setHeight(100.0f);

    auto child2 = std::make_unique<LayoutNode>("child2");
    child2->setDisplay(Display::Block);
    child2->setHeight(150.0f);

    auto child3 = std::make_unique<LayoutNode>("child3");
    child3->setDisplay(Display::Block);
    child3->setHeight(200.0f);

    parent->addChild(std::move(child1));
    parent->addChild(std::move(child2));
    parent->addChild(std::move(child3));

    LayoutConstraints constraints(800.0f, 600.0f);
    BlockLayout::layout(*parent, constraints);

    EXPECT_FLOAT_EQ(parent->getComputedHeight(), 450.0f);

    EXPECT_FLOAT_EQ(parent->getChild(0).getComputedY(), 0.0f);
    EXPECT_FLOAT_EQ(parent->getChild(1).getComputedY(), 100.0f);
    EXPECT_FLOAT_EQ(parent->getChild(2).getComputedY(), 250.0f);
}

// Test 3: Explicit container width - verify child uses it
TEST(BlockLayoutTest, ExplicitContainerWidth) {
    auto parent = std::make_unique<LayoutNode>("parent");
    parent->setDisplay(Display::Block);
    parent->setWidth(400.0f);

    auto child = std::make_unique<LayoutNode>("child");
    child->setDisplay(Display::Block);
    child->setHeight(100.0f);

    parent->addChild(std::move(child));

    LayoutConstraints constraints(800.0f, 600.0f);
    BlockLayout::layout(*parent, constraints);

    EXPECT_FLOAT_EQ(parent->getComputedWidth(), 400.0f);
    EXPECT_FLOAT_EQ(parent->getChild(0).getComputedWidth(), 400.0f);
}

// Test 4: Auto container width - verify fills available space
TEST(BlockLayoutTest, AutoContainerWidth) {
    auto parent = std::make_unique<LayoutNode>("parent");
    parent->setDisplay(Display::Block);

    auto child = std::make_unique<LayoutNode>("child");
    child->setDisplay(Display::Block);
    child->setHeight(100.0f);

    parent->addChild(std::move(child));

    LayoutConstraints constraints(1200.0f, 600.0f);
    BlockLayout::layout(*parent, constraints);

    EXPECT_FLOAT_EQ(parent->getComputedWidth(), 1200.0f);
    EXPECT_FLOAT_EQ(parent->getChild(0).getComputedWidth(), 1200.0f);
}

// Test 5: Explicit container height - verify it's used
TEST(BlockLayoutTest, ExplicitContainerHeight) {
    auto parent = std::make_unique<LayoutNode>("parent");
    parent->setDisplay(Display::Block);
    parent->setHeight(500.0f);

    auto child = std::make_unique<LayoutNode>("child");
    child->setDisplay(Display::Block);
    child->setHeight(100.0f);

    parent->addChild(std::move(child));

    LayoutConstraints constraints(800.0f, 600.0f);
    BlockLayout::layout(*parent, constraints);

    EXPECT_FLOAT_EQ(parent->getComputedHeight(), 500.0f);
    EXPECT_FLOAT_EQ(parent->getChild(0).getComputedHeight(), 100.0f);
}

// Test 6: Auto container height - verify sum of children
TEST(BlockLayoutTest, AutoContainerHeight) {
    auto parent = std::make_unique<LayoutNode>("parent");
    parent->setDisplay(Display::Block);

    auto child1 = std::make_unique<LayoutNode>("child1");
    child1->setDisplay(Display::Block);
    child1->setHeight(100.0f);

    auto child2 = std::make_unique<LayoutNode>("child2");
    child2->setDisplay(Display::Block);
    child2->setHeight(200.0f);

    parent->addChild(std::move(child1));
    parent->addChild(std::move(child2));

    LayoutConstraints constraints(800.0f, 600.0f);
    BlockLayout::layout(*parent, constraints);

    EXPECT_FLOAT_EQ(parent->getComputedHeight(), 300.0f);
}

// Test 7: Children with margins - verify spacing
TEST(BlockLayoutTest, ChildrenWithMargins) {
    auto parent = std::make_unique<LayoutNode>("parent");
    parent->setDisplay(Display::Block);

    auto child1 = std::make_unique<LayoutNode>("child1");
    child1->setDisplay(Display::Block);
    child1->setHeight(100.0f);
    child1->setMargin(EdgeValues(10.0f));

    auto child2 = std::make_unique<LayoutNode>("child2");
    child2->setDisplay(Display::Block);
    child2->setHeight(100.0f);
    child2->setMargin(EdgeValues(20.0f));

    parent->addChild(std::move(child1));
    parent->addChild(std::move(child2));

    LayoutConstraints constraints(800.0f, 600.0f);
    BlockLayout::layout(*parent, constraints);

    EXPECT_FLOAT_EQ(parent->getChild(0).getComputedY(), 10.0f);
    EXPECT_FLOAT_EQ(parent->getChild(1).getComputedY(), 140.0f);
    EXPECT_FLOAT_EQ(parent->getComputedHeight(), 260.0f);
    EXPECT_FLOAT_EQ(parent->getChild(0).getComputedWidth(), 780.0f);
    EXPECT_FLOAT_EQ(parent->getChild(1).getComputedWidth(), 760.0f);
}

// Test 8: Children with padding and border - verify in total size
TEST(BlockLayoutTest, ChildrenWithPaddingAndBorder) {
    auto parent = std::make_unique<LayoutNode>("parent");
    parent->setDisplay(Display::Block);
    parent->setPadding(EdgeValues(10.0f));
    parent->setBorder(EdgeValues(5.0f));

    auto child = std::make_unique<LayoutNode>("child");
    child->setDisplay(Display::Block);
    child->setHeight(100.0f);
    child->setPadding(EdgeValues(8.0f));
    child->setBorder(EdgeValues(2.0f));

    parent->addChild(std::move(child));

    LayoutConstraints constraints(800.0f, 600.0f);
    BlockLayout::layout(*parent, constraints);

    EXPECT_FLOAT_EQ(parent->getComputedWidth(), 800.0f);
    EXPECT_FLOAT_EQ(parent->getChild(0).getComputedX(), 15.0f);
    // Child width: parent content (770) - child horizontal space (20) + child horizontal space (20) = 770
    EXPECT_FLOAT_EQ(parent->getChild(0).getComputedWidth(), 770.0f);
    EXPECT_FLOAT_EQ(parent->getChild(0).getComputedHeight(), 120.0f);
    EXPECT_FLOAT_EQ(parent->getComputedHeight(), 150.0f);
}

// Test 9: Empty container - verify height is 0
TEST(BlockLayoutTest, EmptyContainer) {
    auto parent = std::make_unique<LayoutNode>("parent");
    parent->setDisplay(Display::Block);

    LayoutConstraints constraints(800.0f, 600.0f);
    BlockLayout::layout(*parent, constraints);

    EXPECT_FLOAT_EQ(parent->getComputedWidth(), 800.0f);
    EXPECT_FLOAT_EQ(parent->getComputedHeight(), 0.0f);
}

// Test 10: Nested blocks - verify recursion works
TEST(BlockLayoutTest, NestedBlocks) {
    auto parent = std::make_unique<LayoutNode>("parent");
    parent->setDisplay(Display::Block);

    auto middle = std::make_unique<LayoutNode>("middle");
    middle->setDisplay(Display::Block);
    middle->setHeight(200.0f);

    auto inner = std::make_unique<LayoutNode>("inner");
    inner->setDisplay(Display::Block);
    inner->setHeight(100.0f);

    middle->addChild(std::move(inner));
    parent->addChild(std::move(middle));

    LayoutConstraints constraints(800.0f, 600.0f);
    BlockLayout::layout(*parent, constraints);

    EXPECT_FLOAT_EQ(parent->getComputedHeight(), 200.0f);

    const LayoutNode& middle_ref = parent->getChild(0);
    EXPECT_FLOAT_EQ(middle_ref.getComputedY(), 0.0f);
    EXPECT_FLOAT_EQ(middle_ref.getComputedHeight(), 200.0f);

    const LayoutNode& inner_ref = middle_ref.getChild(0);
    EXPECT_FLOAT_EQ(inner_ref.getComputedY(), 0.0f);
    EXPECT_FLOAT_EQ(inner_ref.getComputedHeight(), 100.0f);

    EXPECT_FLOAT_EQ(parent->getComputedWidth(), 800.0f);
    EXPECT_FLOAT_EQ(middle_ref.getComputedWidth(), 800.0f);
    EXPECT_FLOAT_EQ(inner_ref.getComputedWidth(), 800.0f);
}
