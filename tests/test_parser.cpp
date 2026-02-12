#include <gtest/gtest.h>
#include "parser/LayoutParser.h"
#include <memory>

// ============================================================================
// TEST 1: Parse single node
// ============================================================================
TEST(LayoutParserTest, SingleNode)
{
    std::string input = R"(
        root {
        }
    )";

    LayoutParser parser;
    auto root = parser.parseString(input);

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->getId(), "root");
    EXPECT_EQ(root->getChildCount(), 0);
}

// ============================================================================
// TEST 2: Parse node with properties
// ============================================================================
TEST(LayoutParserTest, NodeWithProperties)
{
    std::string input = R"(
        container {
            display: flex;
            width: 800;
            height: 600;
        }
    )";

    LayoutParser parser;
    auto root = parser.parseString(input);

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->getDisplay(), Display::Flex);
    EXPECT_TRUE(root->hasExplicitWidth());
    EXPECT_FLOAT_EQ(root->getWidth().value(), 800.0f);
    EXPECT_TRUE(root->hasExplicitHeight());
    EXPECT_FLOAT_EQ(root->getHeight().value(), 600.0f);
}

// ============================================================================
// TEST 3: Parse nested nodes
// ============================================================================
TEST(LayoutParserTest, NestedNodes)
{
    std::string input = R"(
        parent {
            display: block;

            child {
                width: 200;
                height: 100;
            }
        }
    )";

    LayoutParser parser;
    auto root = parser.parseString(input);

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->getId(), "parent");
    EXPECT_EQ(root->getChildCount(), 1);

    const LayoutNode &child = root->getChild(0);
    EXPECT_EQ(child.getId(), "child");
    EXPECT_FLOAT_EQ(child.getWidth().value(), 200.0f);
    EXPECT_FLOAT_EQ(child.getHeight().value(), 100.0f);
}

// ============================================================================
// TEST 4: Parse multiple children
// ============================================================================
TEST(LayoutParserTest, MultipleChildren)
{
    std::string input = R"(
        container {
            child1 {
                width: 100;
            }
            child2 {
                width: 200;
            }
            child3 {
                width: 300;
            }
        }
    )";

    LayoutParser parser;
    auto root = parser.parseString(input);

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->getChildCount(), 3);

    EXPECT_EQ(root->getChild(0).getId(), "child1");
    EXPECT_EQ(root->getChild(1).getId(), "child2");
    EXPECT_EQ(root->getChild(2).getId(), "child3");

    EXPECT_FLOAT_EQ(root->getChild(0).getWidth().value(), 100.0f);
    EXPECT_FLOAT_EQ(root->getChild(1).getWidth().value(), 200.0f);
    EXPECT_FLOAT_EQ(root->getChild(2).getWidth().value(), 300.0f);
}

// ============================================================================
// TEST 5: Parse all property types
// ============================================================================
TEST(LayoutParserTest, AllPropertyTypes)
{
    std::string input = R"(
        node {
            display: flex;
            flex-direction: column;
            justify-content: center;
            align-items: stretch;
            width: 800;
            height: 600;
            margin: 10;
            padding: 20;
            border: 5;
        }
    )";

    LayoutParser parser;
    auto root = parser.parseString(input);

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->getDisplay(), Display::Flex);
    EXPECT_EQ(root->getFlexDirection(), FlexDirection::Column);
    EXPECT_EQ(root->getJustifyContent(), JustifyContent::Center);
    EXPECT_EQ(root->getAlignItems(), AlignItems::Stretch);

    EXPECT_FLOAT_EQ(root->getWidth().value(), 800.0f);
    EXPECT_FLOAT_EQ(root->getHeight().value(), 600.0f);

    EXPECT_FLOAT_EQ(root->getMargin().top(), 10.0f);
    EXPECT_FLOAT_EQ(root->getPadding().top(), 20.0f);
    EXPECT_FLOAT_EQ(root->getBorder().top(), 5.0f);
}

// ============================================================================
// TEST 6: Parse edge values - single value
// ============================================================================
TEST(LayoutParserTest, EdgeValuesSingle)
{
    std::string input = R"(
        node {
            margin: 15;
        }
    )";

    LayoutParser parser;
    auto root = parser.parseString(input);

    EXPECT_FLOAT_EQ(root->getMargin().top(), 15.0f);
    EXPECT_FLOAT_EQ(root->getMargin().right(), 15.0f);
    EXPECT_FLOAT_EQ(root->getMargin().bottom(), 15.0f);
    EXPECT_FLOAT_EQ(root->getMargin().left(), 15.0f);
}

// ============================================================================
// TEST 7: Parse edge values - two values
// ============================================================================
TEST(LayoutParserTest, EdgeValuesTwoValues)
{
    std::string input = R"(
        node {
            padding: 10 20;
        }
    )";

    LayoutParser parser;
    auto root = parser.parseString(input);

    EXPECT_FLOAT_EQ(root->getPadding().top(), 10.0f);    // vertical
    EXPECT_FLOAT_EQ(root->getPadding().right(), 20.0f);  // horizontal
    EXPECT_FLOAT_EQ(root->getPadding().bottom(), 10.0f); // vertical
    EXPECT_FLOAT_EQ(root->getPadding().left(), 20.0f);   // horizontal
}

// ============================================================================
// TEST 8: Parse edge values - four values
// ============================================================================
TEST(LayoutParserTest, EdgeValuesFourValues)
{
    std::string input = R"(
        node {
            margin: 10 20 30 40;
        }
    )";

    LayoutParser parser;
    auto root = parser.parseString(input);

    EXPECT_FLOAT_EQ(root->getMargin().top(), 10.0f);
    EXPECT_FLOAT_EQ(root->getMargin().right(), 20.0f);
    EXPECT_FLOAT_EQ(root->getMargin().bottom(), 30.0f);
    EXPECT_FLOAT_EQ(root->getMargin().left(), 40.0f);
}

// ============================================================================
// TEST 9: Parse deeply nested structure
// ============================================================================
TEST(LayoutParserTest, DeeplyNested)
{
    std::string input = R"(
        root {
            level1 {
                level2 {
                    level3 {
                        width: 100;
                    }
                }
            }
        }
    )";

    LayoutParser parser;
    auto root = parser.parseString(input);

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->getChildCount(), 1);

    const LayoutNode &level1 = root->getChild(0);
    EXPECT_EQ(level1.getChildCount(), 1);

    const LayoutNode &level2 = level1.getChild(0);
    EXPECT_EQ(level2.getChildCount(), 1);

    const LayoutNode &level3 = level2.getChild(0);
    EXPECT_FLOAT_EQ(level3.getWidth().value(), 100.0f);
}

// ============================================================================
// TEST 10: Comments are ignored
// ============================================================================
TEST(LayoutParserTest, CommentsIgnored)
{
    std::string input = R"(
        # This is a comment
        root {
            // Another comment style
            width: 100;
            # Comment in middle
            height: 200;
        }
    )";

    LayoutParser parser;
    auto root = parser.parseString(input);

    ASSERT_NE(root, nullptr);
    EXPECT_FLOAT_EQ(root->getWidth().value(), 100.0f);
    EXPECT_FLOAT_EQ(root->getHeight().value(), 200.0f);
}

// ============================================================================
// TEST 11: Error - Missing node identifier
// ============================================================================
TEST(LayoutParserTest, ErrorMissingIdentifier)
{
    std::string input = R"(
        {
            width: 100;
        }
    )";

    LayoutParser parser;
    EXPECT_THROW(parser.parseString(input), ParseError);
}

// ============================================================================
// TEST 12: Error - Unclosed brace
// ============================================================================
TEST(LayoutParserTest, ErrorUnclosedBrace)
{
    std::string input = R"(
        root {
            child {
                width: 100;
            }
        # Missing closing brace
    )";

    LayoutParser parser;
    EXPECT_THROW(parser.parseString(input), ParseError);
}

// ============================================================================
// TEST 13: Error - Extra closing brace
// ============================================================================
TEST(LayoutParserTest, ErrorExtraClosingBrace)
{
    std::string input = R"(
        root {
            width: 100;
        }
        }
    )";

    LayoutParser parser;
    EXPECT_THROW(parser.parseString(input), ParseError);
}

// ============================================================================
// TEST 14: Error - Property outside node
// ============================================================================
TEST(LayoutParserTest, ErrorPropertyOutsideNode)
{
    std::string input = R"(
        width: 100;
        root {
        }
    )";

    LayoutParser parser;
    EXPECT_THROW(parser.parseString(input), ParseError);
}

// ============================================================================
// TEST 15: Error - Invalid property value
// ============================================================================
TEST(LayoutParserTest, ErrorInvalidPropertyValue)
{
    std::string input = R"(
        root {
            display: invalid;
        }
    )";

    LayoutParser parser;
    EXPECT_THROW(parser.parseString(input), ParseError);
}

// ============================================================================
// TEST 16: Error - Invalid number format
// ============================================================================
TEST(LayoutParserTest, ErrorInvalidNumber)
{
    std::string input = R"(
        root {
            width: not-a-number;
        }
    )";

    LayoutParser parser;
    EXPECT_THROW(parser.parseString(input), ParseError);
}

// ============================================================================
// TEST 17: Complete example - Flex layout
// ============================================================================
TEST(LayoutParserTest, CompleteFlexExample)
{
    std::string input = R"(
        container {
            display: flex;
            flex-direction: row;
            justify-content: space-between;
            width: 800;
            height: 600;

            header {
                display: block;
                width: 800;
                height: 100;
                margin: 0 0 10 0;
            }

            sidebar {
                display: block;
                width: 200;
                height: 400;
                padding: 10;
            }

            main {
                display: block;
                width: 500;
                height: 400;
                padding: 20;
            }
        }
    )";

    LayoutParser parser;
    auto root = parser.parseString(input);

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->getId(), "container");
    EXPECT_EQ(root->getDisplay(), Display::Flex);
    EXPECT_EQ(root->getFlexDirection(), FlexDirection::Row);
    EXPECT_EQ(root->getJustifyContent(), JustifyContent::SpaceBetween);
    EXPECT_EQ(root->getChildCount(), 3);

    const LayoutNode &header = root->getChild(0);
    EXPECT_EQ(header.getId(), "header");
    EXPECT_FLOAT_EQ(header.getHeight().value(), 100.0f);
    EXPECT_FLOAT_EQ(header.getMargin().bottom(), 10.0f);

    const LayoutNode &sidebar = root->getChild(1);
    EXPECT_EQ(sidebar.getId(), "sidebar");
    EXPECT_FLOAT_EQ(sidebar.getPadding().top(), 10.0f);

    const LayoutNode &main = root->getChild(2);
    EXPECT_EQ(main.getId(), "main");
    EXPECT_FLOAT_EQ(main.getPadding().top(), 20.0f);
}