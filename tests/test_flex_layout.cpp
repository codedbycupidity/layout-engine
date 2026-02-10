#include <gtest/gtest.h>
#include "layout/FlexLayout.h"
#include "core/LayoutNode.h"
#include "layout/LayoutConstraints.h"
#include <memory>

// Test 1: Single item, flex-start → positioned at start
TEST(FlexLayoutTest, SingleItemFlexStart)
{
    auto container = std::make_unique<LayoutNode>("container");
    container->setDisplay(Display::Flex);
    container->setFlexDirection(FlexDirection::Row);
    container->setJustifyContent(JustifyContent::FlexStart);

    auto child = std::make_unique<LayoutNode>("child");
    child->setDisplay(Display::Block);
    child->setWidth(100.0f);
    child->setHeight(50.0f);

    container->addChild(std::move(child));

    LayoutConstraints constraints(800.0f, 600.0f);
    FlexLayout::layout(*container, constraints);

    // Container should have auto width = child width
    EXPECT_FLOAT_EQ(container->getComputedWidth(), 100.0f);

    // Child positioned at start (0, 0)
    const LayoutNode &child_ref = container->getChild(0);
    EXPECT_FLOAT_EQ(child_ref.getComputedX(), 0.0f);
    EXPECT_FLOAT_EQ(child_ref.getComputedY(), 0.0f);
}

// Test 2: Two items, flex-start → no spacing
TEST(FlexLayoutTest, TwoItemsFlexStart)
{
    auto container = std::make_unique<LayoutNode>("container");
    container->setDisplay(Display::Flex);
    container->setFlexDirection(FlexDirection::Row);
    container->setJustifyContent(JustifyContent::FlexStart);

    auto child1 = std::make_unique<LayoutNode>("child1");
    child1->setDisplay(Display::Block);
    child1->setWidth(100.0f);
    child1->setHeight(50.0f);

    auto child2 = std::make_unique<LayoutNode>("child2");
    child2->setDisplay(Display::Block);
    child2->setWidth(150.0f);
    child2->setHeight(50.0f);

    container->addChild(std::move(child1));
    container->addChild(std::move(child2));

    LayoutConstraints constraints(800.0f, 600.0f);
    FlexLayout::layout(*container, constraints);

    // Child1 at x=0, Child2 at x=100 (no spacing)
    EXPECT_FLOAT_EQ(container->getChild(0).getComputedX(), 0.0f);
    EXPECT_FLOAT_EQ(container->getChild(1).getComputedX(), 100.0f);
}

// Test 3: Two items, center → centered with no spacing
TEST(FlexLayoutTest, TwoItemsCenter)
{
    auto container = std::make_unique<LayoutNode>("container");
    container->setDisplay(Display::Flex);
    container->setFlexDirection(FlexDirection::Row);
    container->setJustifyContent(JustifyContent::Center);
    container->setWidth(800.0f); // Explicit width

    auto child1 = std::make_unique<LayoutNode>("child1");
    child1->setDisplay(Display::Block);
    child1->setWidth(100.0f);
    child1->setHeight(50.0f);

    auto child2 = std::make_unique<LayoutNode>("child2");
    child2->setDisplay(Display::Block);
    child2->setWidth(100.0f);
    child2->setHeight(50.0f);

    container->addChild(std::move(child1));
    container->addChild(std::move(child2));

    LayoutConstraints constraints(800.0f, 600.0f);
    FlexLayout::layout(*container, constraints);

    // Total item width = 200px, available = 800px
    // Center offset = (800 - 200) / 2 = 300
    EXPECT_FLOAT_EQ(container->getChild(0).getComputedX(), 300.0f);
    EXPECT_FLOAT_EQ(container->getChild(1).getComputedX(), 400.0f);
}

// Test 4: Two items, flex-end → positioned at end
TEST(FlexLayoutTest, TwoItemsFlexEnd)
{
    auto container = std::make_unique<LayoutNode>("container");
    container->setDisplay(Display::Flex);
    container->setFlexDirection(FlexDirection::Row);
    container->setJustifyContent(JustifyContent::FlexEnd);
    container->setWidth(800.0f);

    auto child1 = std::make_unique<LayoutNode>("child1");
    child1->setDisplay(Display::Block);
    child1->setWidth(100.0f);
    child1->setHeight(50.0f);

    auto child2 = std::make_unique<LayoutNode>("child2");
    child2->setDisplay(Display::Block);
    child2->setWidth(100.0f);
    child2->setHeight(50.0f);

    container->addChild(std::move(child1));
    container->addChild(std::move(child2));

    LayoutConstraints constraints(800.0f, 600.0f);
    FlexLayout::layout(*container, constraints);

    // Items at end: 800 - 200 = 600
    EXPECT_FLOAT_EQ(container->getChild(0).getComputedX(), 600.0f);
    EXPECT_FLOAT_EQ(container->getChild(1).getComputedX(), 700.0f);
}

// Test 5: Two items, space-between → spacing distributed
TEST(FlexLayoutTest, TwoItemsSpaceBetween)
{
    auto container = std::make_unique<LayoutNode>("container");
    container->setDisplay(Display::Flex);
    container->setFlexDirection(FlexDirection::Row);
    container->setJustifyContent(JustifyContent::SpaceBetween);
    container->setWidth(800.0f);

    auto child1 = std::make_unique<LayoutNode>("child1");
    child1->setDisplay(Display::Block);
    child1->setWidth(100.0f);
    child1->setHeight(50.0f);

    auto child2 = std::make_unique<LayoutNode>("child2");
    child2->setDisplay(Display::Block);
    child2->setWidth(100.0f);
    child2->setHeight(50.0f);

    container->addChild(std::move(child1));
    container->addChild(std::move(child2));

    LayoutConstraints constraints(800.0f, 600.0f);
    FlexLayout::layout(*container, constraints);

    // Space between = (800 - 200) / 1 = 600
    EXPECT_FLOAT_EQ(container->getChild(0).getComputedX(), 0.0f);
    EXPECT_FLOAT_EQ(container->getChild(1).getComputedX(), 700.0f); // 0 + 100 + 600
}

// Test 6: Three items, space-between
TEST(FlexLayoutTest, ThreeItemsSpaceBetween)
{
    auto container = std::make_unique<LayoutNode>("container");
    container->setDisplay(Display::Flex);
    container->setFlexDirection(FlexDirection::Row);
    container->setJustifyContent(JustifyContent::SpaceBetween);
    container->setWidth(800.0f);

    auto child1 = std::make_unique<LayoutNode>("child1");
    child1->setDisplay(Display::Block);
    child1->setWidth(100.0f);
    child1->setHeight(50.0f);

    auto child2 = std::make_unique<LayoutNode>("child2");
    child2->setDisplay(Display::Block);
    child2->setWidth(100.0f);
    child2->setHeight(50.0f);

    auto child3 = std::make_unique<LayoutNode>("child3");
    child3->setDisplay(Display::Block);
    child3->setWidth(100.0f);
    child3->setHeight(50.0f);

    container->addChild(std::move(child1));
    container->addChild(std::move(child2));
    container->addChild(std::move(child3));

    LayoutConstraints constraints(800.0f, 600.0f);
    FlexLayout::layout(*container, constraints);

    // Space between = (800 - 300) / 2 = 250
    EXPECT_FLOAT_EQ(container->getChild(0).getComputedX(), 0.0f);
    EXPECT_FLOAT_EQ(container->getChild(1).getComputedX(), 350.0f); // 0 + 100 + 250
    EXPECT_FLOAT_EQ(container->getChild(2).getComputedX(), 700.0f); // 350 + 100 + 250
}

// Test 7: Items with different heights, align-items flex-start
TEST(FlexLayoutTest, DifferentHeightsAlignStart)
{
    auto container = std::make_unique<LayoutNode>("container");
    container->setDisplay(Display::Flex);
    container->setFlexDirection(FlexDirection::Row);
    container->setJustifyContent(JustifyContent::FlexStart);
    container->setAlignItems(AlignItems::FlexStart);

    auto child1 = std::make_unique<LayoutNode>("child1");
    child1->setDisplay(Display::Block);
    child1->setWidth(100.0f);
    child1->setHeight(50.0f);

    auto child2 = std::make_unique<LayoutNode>("child2");
    child2->setDisplay(Display::Block);
    child2->setWidth(100.0f);
    child2->setHeight(100.0f); // Taller

    container->addChild(std::move(child1));
    container->addChild(std::move(child2));

    LayoutConstraints constraints(800.0f, 600.0f);
    FlexLayout::layout(*container, constraints);

    // Both items aligned to top
    EXPECT_FLOAT_EQ(container->getChild(0).getComputedY(), 0.0f);
    EXPECT_FLOAT_EQ(container->getChild(1).getComputedY(), 0.0f);

    // Container height = tallest item
    EXPECT_FLOAT_EQ(container->getComputedHeight(), 100.0f);
}

// Test 8: Items with different heights, align-items center
TEST(FlexLayoutTest, DifferentHeightsAlignCenter)
{
    auto container = std::make_unique<LayoutNode>("container");
    container->setDisplay(Display::Flex);
    container->setFlexDirection(FlexDirection::Row);
    container->setJustifyContent(JustifyContent::FlexStart);
    container->setAlignItems(AlignItems::Center);

    auto child1 = std::make_unique<LayoutNode>("child1");
    child1->setDisplay(Display::Block);
    child1->setWidth(100.0f);
    child1->setHeight(50.0f);

    auto child2 = std::make_unique<LayoutNode>("child2");
    child2->setDisplay(Display::Block);
    child2->setWidth(100.0f);
    child2->setHeight(100.0f);

    container->addChild(std::move(child1));
    container->addChild(std::move(child2));

    LayoutConstraints constraints(800.0f, 600.0f);
    FlexLayout::layout(*container, constraints);

    // Container height = 100 (tallest)
    // Child1 (50px) centered: (100 - 50) / 2 = 25
    // Child2 (100px) centered: (100 - 100) / 2 = 0
    EXPECT_FLOAT_EQ(container->getChild(0).getComputedY(), 25.0f);
    EXPECT_FLOAT_EQ(container->getChild(1).getComputedY(), 0.0f);
}

// Test 9: Items with different heights, align-items flex-end
TEST(FlexLayoutTest, DifferentHeightsAlignEnd)
{
    auto container = std::make_unique<LayoutNode>("container");
    container->setDisplay(Display::Flex);
    container->setFlexDirection(FlexDirection::Row);
    container->setJustifyContent(JustifyContent::FlexStart);
    container->setAlignItems(AlignItems::FlexEnd);

    auto child1 = std::make_unique<LayoutNode>("child1");
    child1->setDisplay(Display::Block);
    child1->setWidth(100.0f);
    child1->setHeight(50.0f);

    auto child2 = std::make_unique<LayoutNode>("child2");
    child2->setDisplay(Display::Block);
    child2->setWidth(100.0f);
    child2->setHeight(100.0f);

    container->addChild(std::move(child1));
    container->addChild(std::move(child2));

    LayoutConstraints constraints(800.0f, 600.0f);
    FlexLayout::layout(*container, constraints);

    // Container height = 100
    // Child1 aligned to bottom: 100 - 50 = 50
    // Child2 aligned to bottom: 100 - 100 = 0
    EXPECT_FLOAT_EQ(container->getChild(0).getComputedY(), 50.0f);
    EXPECT_FLOAT_EQ(container->getChild(1).getComputedY(), 0.0f);
}

// Test 10: Flex-direction column
TEST(FlexLayoutTest, ColumnDirection)
{
    auto container = std::make_unique<LayoutNode>("container");
    container->setDisplay(Display::Flex);
    container->setFlexDirection(FlexDirection::Column);
    container->setJustifyContent(JustifyContent::FlexStart);
    container->setAlignItems(AlignItems::FlexStart);

    auto child1 = std::make_unique<LayoutNode>("child1");
    child1->setDisplay(Display::Block);
    child1->setWidth(100.0f);
    child1->setHeight(50.0f);

    auto child2 = std::make_unique<LayoutNode>("child2");
    child2->setDisplay(Display::Block);
    child2->setWidth(100.0f);
    child2->setHeight(50.0f);

    container->addChild(std::move(child1));
    container->addChild(std::move(child2));

    LayoutConstraints constraints(800.0f, 600.0f);
    FlexLayout::layout(*container, constraints);

    // In column, items stack vertically
    EXPECT_FLOAT_EQ(container->getChild(0).getComputedY(), 0.0f);
    EXPECT_FLOAT_EQ(container->getChild(1).getComputedY(), 50.0f);

    // Both at X = 0
    EXPECT_FLOAT_EQ(container->getChild(0).getComputedX(), 0.0f);
    EXPECT_FLOAT_EQ(container->getChild(1).getComputedX(), 0.0f);

    // Container dimensions
    EXPECT_FLOAT_EQ(container->getComputedWidth(), 100.0f);
    EXPECT_FLOAT_EQ(container->getComputedHeight(), 100.0f);
}

// Test 11: Column with space-between
TEST(FlexLayoutTest, ColumnSpaceBetween)
{
    auto container = std::make_unique<LayoutNode>("container");
    container->setDisplay(Display::Flex);
    container->setFlexDirection(FlexDirection::Column);
    container->setJustifyContent(JustifyContent::SpaceBetween);
    container->setHeight(600.0f); // Explicit height

    auto child1 = std::make_unique<LayoutNode>("child1");
    child1->setDisplay(Display::Block);
    child1->setWidth(100.0f);
    child1->setHeight(100.0f);

    auto child2 = std::make_unique<LayoutNode>("child2");
    child2->setDisplay(Display::Block);
    child2->setWidth(100.0f);
    child2->setHeight(100.0f);

    container->addChild(std::move(child1));
    container->addChild(std::move(child2));

    LayoutConstraints constraints(800.0f, 600.0f);
    FlexLayout::layout(*container, constraints);

    // Space between = (600 - 200) / 1 = 400
    EXPECT_FLOAT_EQ(container->getChild(0).getComputedY(), 0.0f);
    EXPECT_FLOAT_EQ(container->getChild(1).getComputedY(), 500.0f); // 0 + 100 + 400
}

// Test 12: Items with margins
TEST(FlexLayoutTest, ItemsWithMargins)
{
    auto container = std::make_unique<LayoutNode>("container");
    container->setDisplay(Display::Flex);
    container->setFlexDirection(FlexDirection::Row);
    container->setJustifyContent(JustifyContent::FlexStart);

    auto child1 = std::make_unique<LayoutNode>("child1");
    child1->setDisplay(Display::Block);
    child1->setWidth(100.0f);
    child1->setHeight(50.0f);
    child1->setMargin(EdgeValues(10.0f)); // 10px all sides

    auto child2 = std::make_unique<LayoutNode>("child2");
    child2->setDisplay(Display::Block);
    child2->setWidth(100.0f);
    child2->setHeight(50.0f);
    child2->setMargin(EdgeValues(10.0f));

    container->addChild(std::move(child1));
    container->addChild(std::move(child2));

    LayoutConstraints constraints(800.0f, 600.0f);
    FlexLayout::layout(*container, constraints);

    // Child1 X = 0 + margin.left = 10
    EXPECT_FLOAT_EQ(container->getChild(0).getComputedX(), 10.0f);

    // Child2 X = 10 + 100 + 10 (child1 right margin) + 10 (child2 left margin) = 130
    EXPECT_FLOAT_EQ(container->getChild(1).getComputedX(), 130.0f);

    // Container width = 120 (child1 with margins) + 120 (child2 with margins) = 240
    EXPECT_FLOAT_EQ(container->getComputedWidth(), 240.0f);
}
