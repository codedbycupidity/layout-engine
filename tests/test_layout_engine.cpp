#include <gtest/gtest.h>
#include "layout/LayoutEngine.h"
#include "core/LayoutNode.h"
#include <memory>

// Test 1: Simple block layout via engine
TEST(LayoutEngineTest, SimpleBlockLayout)
{
    auto root = std::make_unique<LayoutNode>("root");
    root->setDisplay(Display::Block);

    auto child = std::make_unique<LayoutNode>("child");
    child->setDisplay(Display::Block);
    child->setWidth(200.0f);
    child->setHeight(100.0f);

    root->addChild(std::move(child));

    LayoutEngine engine;
    engine.layout(*root, 800.0f, 600.0f);

    // Root should fill viewport
    EXPECT_FLOAT_EQ(root->getComputedWidth(), 800.0f);
    EXPECT_FLOAT_EQ(root->getComputedHeight(), 600.0f);
    EXPECT_FLOAT_EQ(root->getComputedX(), 0.0f);
    EXPECT_FLOAT_EQ(root->getComputedY(), 0.0f);

    // Child should be positioned and sized
    const LayoutNode &child_ref = root->getChild(0);
    EXPECT_FLOAT_EQ(child_ref.getComputedWidth(), 200.0f);
    EXPECT_FLOAT_EQ(child_ref.getComputedHeight(), 100.0f);
    EXPECT_FLOAT_EQ(child_ref.getComputedX(), 0.0f);
    EXPECT_FLOAT_EQ(child_ref.getComputedY(), 0.0f);
}

// Test 2: Simple flex layout via engine
TEST(LayoutEngineTest, SimpleFlexLayout)
{
    auto root = std::make_unique<LayoutNode>("root");
    root->setDisplay(Display::Flex);
    root->setFlexDirection(FlexDirection::Row);
    root->setJustifyContent(JustifyContent::SpaceBetween);

    auto child1 = std::make_unique<LayoutNode>("child1");
    child1->setDisplay(Display::Block);
    child1->setWidth(100.0f);
    child1->setHeight(50.0f);

    auto child2 = std::make_unique<LayoutNode>("child2");
    child2->setDisplay(Display::Block);
    child2->setWidth(100.0f);
    child2->setHeight(50.0f);

    root->addChild(std::move(child1));
    root->addChild(std::move(child2));

    LayoutEngine engine;
    engine.layout(*root, 800.0f, 600.0f);

    // Root should fill viewport
    EXPECT_FLOAT_EQ(root->getComputedWidth(), 800.0f);
    EXPECT_FLOAT_EQ(root->getComputedHeight(), 600.0f);

    // Children should be spaced apart
    EXPECT_FLOAT_EQ(root->getChild(0).getComputedX(), 0.0f);
    EXPECT_FLOAT_EQ(root->getChild(1).getComputedX(), 700.0f);
}

// Test 3: Nested block containers
TEST(LayoutEngineTest, NestedBlocks)
{
    auto root = std::make_unique<LayoutNode>("root");
    root->setDisplay(Display::Block);

    auto middle = std::make_unique<LayoutNode>("middle");
    middle->setDisplay(Display::Block);
    middle->setWidth(400.0f);
    middle->setPadding(EdgeValues(20.0f));

    auto child = std::make_unique<LayoutNode>("child");
    child->setDisplay(Display::Block);
    child->setWidth(100.0f);
    child->setHeight(50.0f);
    child->setMargin(EdgeValues(10.0f));

    middle->addChild(std::move(child));
    root->addChild(std::move(middle));

    LayoutEngine engine;
    engine.layout(*root, 800.0f, 600.0f);

    // Check middle container
    const LayoutNode &middle_ref = root->getChild(0);
    EXPECT_FLOAT_EQ(middle_ref.getComputedWidth(), 440.0f);

    // Check child position
    const LayoutNode &child_ref = middle_ref.getChild(0);
    EXPECT_FLOAT_EQ(child_ref.getComputedX(), 30.0f);
    EXPECT_FLOAT_EQ(child_ref.getComputedY(), 30.0f);
}

// Test 4: Display none handling
TEST(LayoutEngineTest, DisplayNone)
{
    auto root = std::make_unique<LayoutNode>("root");
    root->setDisplay(Display::Block);

    auto visible_child = std::make_unique<LayoutNode>("visible");
    visible_child->setDisplay(Display::Block);
    visible_child->setWidth(200.0f);
    visible_child->setHeight(100.0f);

    auto hidden_child = std::make_unique<LayoutNode>("hidden");
    hidden_child->setDisplay(Display::None);
    hidden_child->setWidth(200.0f);
    hidden_child->setHeight(100.0f);

    root->addChild(std::move(visible_child));
    root->addChild(std::move(hidden_child));

    LayoutEngine engine;
    engine.layout(*root, 800.0f, 600.0f);

    // Visible child should be laid out normally
    const LayoutNode &visible_ref = root->getChild(0);
    EXPECT_FLOAT_EQ(visible_ref.getComputedWidth(), 200.0f);
    EXPECT_FLOAT_EQ(visible_ref.getComputedHeight(), 100.0f);

    // Hidden child should have zero size
    const LayoutNode &hidden_ref = root->getChild(1);
    EXPECT_FLOAT_EQ(hidden_ref.getComputedWidth(), 0.0f);
    EXPECT_FLOAT_EQ(hidden_ref.getComputedHeight(), 0.0f);
}

// Test 5: Root with explicit dimensions
TEST(LayoutEngineTest, RootWithExplicitDimensions)
{
    auto root = std::make_unique<LayoutNode>("root");
    root->setDisplay(Display::Block);
    root->setWidth(1200.0f);
    root->setHeight(800.0f);

    auto child = std::make_unique<LayoutNode>("child");
    child->setDisplay(Display::Block);
    child->setWidth(100.0f);
    child->setHeight(100.0f);

    root->addChild(std::move(child));

    LayoutEngine engine;
    engine.layout(*root, 800.0f, 600.0f);

    // Root should keep its explicit dimensions
    EXPECT_FLOAT_EQ(root->getComputedWidth(), 1200.0f);
    EXPECT_FLOAT_EQ(root->getComputedHeight(), 800.0f);

    // Root should still be at origin
    EXPECT_FLOAT_EQ(root->getComputedX(), 0.0f);
    EXPECT_FLOAT_EQ(root->getComputedY(), 0.0f);
}
