
#include <gtest/gtest.h>
#include "output/OutputFormatter.h"
#include "core/LayoutNode.h"
#include <memory>

// ============================================================================
// TEST 1: Console output - single node
// ============================================================================
TEST(OutputFormatterTest, ConsoleSingleNode)
{
    auto root = std::make_unique<LayoutNode>("root");
    root->setComputedPosition(0.0f, 0.0f);
    root->setComputedSize(800.0f, 600.0f);

    // Just ensure it doesn't crash
    // Visual inspection in actual run
    OutputFormatter::printConsole(*root);

    // INTERVIEW NOTE: Console output is hard to test programmatically
    // In practice, you might:
    // - Redirect std::cout to a stringstream
    // - Compare against expected output
    // - Or just do manual verification
    SUCCEED();
}

// ============================================================================
// TEST 2: Console output - nested nodes
// ============================================================================
TEST(OutputFormatterTest, ConsoleNestedNodes)
{
    auto root = std::make_unique<LayoutNode>("root");
    root->setComputedPosition(0.0f, 0.0f);
    root->setComputedSize(800.0f, 600.0f);

    auto child1 = std::make_unique<LayoutNode>("child1");
    child1->setComputedPosition(10.0f, 10.0f);
    child1->setComputedSize(200.0f, 100.0f);

    auto child2 = std::make_unique<LayoutNode>("child2");
    child2->setComputedPosition(220.0f, 10.0f);
    child2->setComputedSize(300.0f, 150.0f);

    root->addChild(std::move(child1));
    root->addChild(std::move(child2));

    OutputFormatter::printConsole(*root);
    SUCCEED();
}

// ============================================================================
// TEST 3: JSON output - single node
// ============================================================================
TEST(OutputFormatterTest, JSONSingleNode)
{
    auto root = std::make_unique<LayoutNode>("root");
    root->setComputedPosition(0.0f, 0.0f);
    root->setComputedSize(800.0f, 600.0f);

    std::string json = OutputFormatter::toJSON(*root);

    // Verify JSON contains expected fields
    EXPECT_NE(json.find("\"id\""), std::string::npos);
    EXPECT_NE(json.find("\"root\""), std::string::npos);
    EXPECT_NE(json.find("\"x\""), std::string::npos);
    EXPECT_NE(json.find("\"y\""), std::string::npos);
    EXPECT_NE(json.find("\"width\""), std::string::npos);
    EXPECT_NE(json.find("\"height\""), std::string::npos);
    EXPECT_NE(json.find("\"children\""), std::string::npos);

    // Verify values
    EXPECT_NE(json.find("0.0"), std::string::npos); // x and y
    EXPECT_NE(json.find("800.0"), std::string::npos);
    EXPECT_NE(json.find("600.0"), std::string::npos);
}

// ============================================================================
// TEST 4: JSON output - nested nodes
// ============================================================================
TEST(OutputFormatterTest, JSONNestedNodes)
{
    auto root = std::make_unique<LayoutNode>("container");
    root->setComputedPosition(0.0f, 0.0f);
    root->setComputedSize(800.0f, 600.0f);

    auto child = std::make_unique<LayoutNode>("child");
    child->setComputedPosition(10.0f, 20.0f);
    child->setComputedSize(200.0f, 100.0f);

    root->addChild(std::move(child));

    std::string json = OutputFormatter::toJSON(*root);

    // Verify both nodes present
    EXPECT_NE(json.find("\"container\""), std::string::npos);
    EXPECT_NE(json.find("\"child\""), std::string::npos);

    // Verify nested structure
    size_t children_pos = json.find("\"children\"");
    size_t child_id_pos = json.find("\"child\"");
    EXPECT_LT(children_pos, child_id_pos); // "children" comes before child id
}

// ============================================================================
// TEST 5: JSON output - multiple children
// ============================================================================
TEST(OutputFormatterTest, JSONMultipleChildren)
{
    auto root = std::make_unique<LayoutNode>("root");
    root->setComputedPosition(0.0f, 0.0f);
    root->setComputedSize(800.0f, 600.0f);

    auto child1 = std::make_unique<LayoutNode>("child1");
    child1->setComputedPosition(0.0f, 0.0f);
    child1->setComputedSize(100.0f, 100.0f);

    auto child2 = std::make_unique<LayoutNode>("child2");
    child2->setComputedPosition(100.0f, 0.0f);
    child2->setComputedSize(100.0f, 100.0f);

    auto child3 = std::make_unique<LayoutNode>("child3");
    child3->setComputedPosition(200.0f, 0.0f);
    child3->setComputedSize(100.0f, 100.0f);

    root->addChild(std::move(child1));
    root->addChild(std::move(child2));
    root->addChild(std::move(child3));

    std::string json = OutputFormatter::toJSON(*root);

    // Verify all children present
    EXPECT_NE(json.find("\"child1\""), std::string::npos);
    EXPECT_NE(json.find("\"child2\""), std::string::npos);
    EXPECT_NE(json.find("\"child3\""), std::string::npos);
}

// ============================================================================
// TEST 6: JSON output - deeply nested
// ============================================================================
TEST(OutputFormatterTest, JSONDeeplyNested)
{
    auto root = std::make_unique<LayoutNode>("root");
    root->setComputedPosition(0.0f, 0.0f);
    root->setComputedSize(800.0f, 600.0f);

    auto level1 = std::make_unique<LayoutNode>("level1");
    level1->setComputedPosition(10.0f, 10.0f);
    level1->setComputedSize(780.0f, 580.0f);

    auto level2 = std::make_unique<LayoutNode>("level2");
    level2->setComputedPosition(20.0f, 20.0f);
    level2->setComputedSize(760.0f, 560.0f);

    level1->addChild(std::move(level2));
    root->addChild(std::move(level1));

    std::string json = OutputFormatter::toJSON(*root);

    // Verify all levels present
    EXPECT_NE(json.find("\"root\""), std::string::npos);
    EXPECT_NE(json.find("\"level1\""), std::string::npos);
    EXPECT_NE(json.find("\"level2\""), std::string::npos);
}

// ============================================================================
// TEST 7: JSON compact vs pretty
// ============================================================================
TEST(OutputFormatterTest, JSONCompactVsPretty)
{
    auto root = std::make_unique<LayoutNode>("root");
    root->setComputedPosition(0.0f, 0.0f);
    root->setComputedSize(100.0f, 100.0f);

    auto child = std::make_unique<LayoutNode>("child");
    child->setComputedPosition(10.0f, 10.0f);
    child->setComputedSize(50.0f, 50.0f);

    root->addChild(std::move(child));

    std::string pretty = OutputFormatter::toJSON(*root, true);
    std::string compact = OutputFormatter::toJSON(*root, false);

    // Pretty should have newlines
    EXPECT_NE(pretty.find('\n'), std::string::npos);

    // Compact should not have newlines
    EXPECT_EQ(compact.find('\n'), std::string::npos);

    // Compact should be shorter
    EXPECT_LT(compact.length(), pretty.length());
}

// ============================================================================
// TEST 8: Float formatting precision
// ============================================================================
TEST(OutputFormatterTest, FloatPrecision)
{
    auto root = std::make_unique<LayoutNode>("root");
    root->setComputedPosition(10.123456f, 20.987654f);
    root->setComputedSize(100.555555f, 200.444444f);

    std::string json = OutputFormatter::toJSON(*root);

    // Values should be formatted with limited precision
    // Default is 1 decimal place
    EXPECT_NE(json.find("10.1"), std::string::npos);
    EXPECT_NE(json.find("21.0"), std::string::npos);
    EXPECT_NE(json.find("100.6"), std::string::npos);
    EXPECT_NE(json.find("200.4"), std::string::npos);
}

// ============================================================================
// TEST 9: Integration - Full layout to JSON
// ============================================================================
TEST(OutputFormatterTest, IntegrationFullLayout)
{
    // Create a complete layout structure
    auto page = std::make_unique<LayoutNode>("page");
    page->setComputedPosition(0.0f, 0.0f);
    page->setComputedSize(1024.0f, 768.0f);

    auto header = std::make_unique<LayoutNode>("header");
    header->setComputedPosition(0.0f, 0.0f);
    header->setComputedSize(1024.0f, 80.0f);

    auto content = std::make_unique<LayoutNode>("content");
    content->setComputedPosition(0.0f, 80.0f);
    content->setComputedSize(1024.0f, 638.0f);

    auto sidebar = std::make_unique<LayoutNode>("sidebar");
    sidebar->setComputedPosition(0.0f, 0.0f);
    sidebar->setComputedSize(250.0f, 638.0f);

    auto main = std::make_unique<LayoutNode>("main");
    main->setComputedPosition(250.0f, 0.0f);
    main->setComputedSize(774.0f, 638.0f);

    content->addChild(std::move(sidebar));
    content->addChild(std::move(main));

    auto footer = std::make_unique<LayoutNode>("footer");
    footer->setComputedPosition(0.0f, 718.0f);
    footer->setComputedSize(1024.0f, 50.0f);

    page->addChild(std::move(header));
    page->addChild(std::move(content));
    page->addChild(std::move(footer));

    std::string json = OutputFormatter::toJSON(*page);

    // Verify structure
    EXPECT_NE(json.find("\"page\""), std::string::npos);
    EXPECT_NE(json.find("\"header\""), std::string::npos);
    EXPECT_NE(json.find("\"content\""), std::string::npos);
    EXPECT_NE(json.find("\"sidebar\""), std::string::npos);
    EXPECT_NE(json.find("\"main\""), std::string::npos);
    EXPECT_NE(json.find("\"footer\""), std::string::npos);

    // Print for visual inspection
    std::cout << "\n=== Full Layout JSON ===\n" << json << "\n" << std::endl;
}