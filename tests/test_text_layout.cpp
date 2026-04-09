#include <gtest/gtest.h>
#include "text/TextLayout.h"
#include "text/FontMetrics.h"

class TextLayoutTest : public ::testing::Test
{

protected:
    FontMetrics metrics;
    TextLayout layout;

    void SetUp() override
    {
        metrics = FontMetrics(10.0f, 20.0f, 0.0f); // 10px chars, 20pc line height
        layout = TextLayout(metrics);
    }
};

TEST(FontMetricsTest, DefaultConstructor)
{
    FontMetrics fm;
    EXPECT_EQ(fm.getCharWidth(), 8.0f);
    EXPECT_EQ(fm.getLineHeight(), 20.0f);
    EXPECT_EQ(fm.getLetterSpacing(), 0.0f);
}

TEST(FontMetricsTest, CustomConstructor)
{
    FontMetrics fm(12.0f, 24.0f, 1.0f);
    EXPECT_EQ(fm.getCharWidth(), 12.0f);
    EXPECT_EQ(fm.getLineHeight(), 24.0f);
    EXPECT_EQ(fm.getLetterSpacing(), 1.0f);
}

TEST(FontMetricsTest, WordWidthNoSpacing)
{
    FontMetrics fm(10.0f, 20.0f, 0.0f);
    EXPECT_EQ(fm.getWordWidth("hello"), 50.0f); // 5 chars * 10px
}

TEST(FontMetricsTest, WordWidthWithSpacing)
{
    FontMetrics fm(10.0f, 20.0f, 2.0f);
    // "hello" = 5 chars * 10px + 4 spaces * 2px = 50 + 8 = 58
    EXPECT_EQ(fm.getWordWidth("hello"), 58.0f);
}

TEST(FontMetricsTest, EmptyWord)
{
    FontMetrics fm(10.0f, 20.0f, 0.0f);
    EXPECT_EQ(fm.getWordWidth(""), 0.0f);
}

TEST_F(TextLayoutTest, SingleWord)
{
    auto result = layout.layout("Hello", 100.0f);
    ASSERT_EQ(result.lines.size(), 1);
    EXPECT_EQ(result.lines[0].words.size(), 1);
    EXPECT_EQ(result.lines[0].words[0], "Hello");
    EXPECT_EQ(result.lines[0].width, 50.0f);
}

TEST_F(TextLayoutTest, SingleLineFitsExactly)
{
    auto result = layout.layout("Hi there", 80.0f);
    ASSERT_EQ(result.lines.size(), 1);
    EXPECT_EQ(result.lines[0].words.size(), 2);
}

TEST_F(TextLayoutTest, TwoLinesBreaking)
{
    auto result = layout.layout("Hello World", 80.0f);
    ASSERT_EQ(result.lines.size(), 2);
    EXPECT_EQ(result.lines[0].words[0], "Hello");
    EXPECT_EQ(result.lines[1].words[0], "World");
}

TEST_F(TextLayoutTest, MultipleLines)
{
    auto result = layout.layout("a b c d", 25.0f);
    ASSERT_EQ(result.lines.size(), 4);
}

TEST_F(TextLayoutTest, LeftAlignment)
{
    auto result = layout.layout("a b c d", 25.0f);
    ASSERT_EQ(result.lines.size(), 4);
    EXPECT_EQ(result.lines[0].x_offset, 0.0f);
}

TEST_F(TextLayoutTest, RightAlignment)
{
    auto result = layout.layout("Hi", 100.0f, TextAlign::Right);
    ASSERT_EQ(result.lines.size(), 1);
    EXPECT_EQ(result.lines[0].x_offset, 80.0f);
}

TEST_F(TextLayoutTest, CenterAlignment)
{
    auto result = layout.layout("Hi", 100.0f, TextAlign::Center);
    ASSERT_EQ(result.lines.size(), 1);
    EXPECT_EQ(result.lines[0].x_offset, 40.0f);
}

TEST_F(TextLayoutTest, LinePositions)
{
    auto result = layout.layout("A B C", 25.0f);

    ASSERT_EQ(result.lines.size(), 3);
    EXPECT_EQ(result.lines[0].y_position, 0.0f);
    EXPECT_EQ(result.lines[1].y_position, 20.0f);
    EXPECT_EQ(result.lines[2].y_position, 40.0f);
    EXPECT_EQ(result.total_height, 60.0f);
}

TEST_F(TextLayoutTest, TotalWidthTracking)
{
    auto result = layout.layout("A BB CCC", 25.0f);
    EXPECT_EQ(result.total_width, 30.0f);
}

TEST_F(TextLayoutTest, VeryLongWord)
{
    auto result = layout.layout("Supercalifragilisticexpialidocious", 50.0f);
    ASSERT_EQ(result.lines.size(), 1);
    EXPECT_GT(result.lines[0].width, 50.0f); // Word exceeds container width
}

TEST_F(TextLayoutTest, MultipleSpaces)
{
    auto result = layout.layout("A    B", 100.0f);
    ASSERT_EQ(result.lines.size(), 1);
    EXPECT_EQ(result.lines[0].words.size(), 2);
}

TEST_F(TextLayoutTest, JustifyAlignment)
{
    auto result = layout.layout("A B C D", 35.0f, TextAlign::Justify);
    ASSERT_EQ(result.lines.size(), 2);
    EXPECT_TRUE(result.lines[0].is_justified);
    EXPECT_EQ(result.lines[0].x_offset, 0.0f);
}

TEST_F(TextLayoutTest, JustifyLastLineNotJustified)
{
    auto result = layout.layout("A B C D", 35.0f, TextAlign::Justify);
    ASSERT_GT(result.lines.size(), 1);

    for (size_t i = 0; i < result.lines.size() - 1; i++)
    {
        EXPECT_TRUE(result.lines[i].is_justified);
    }
    EXPECT_FALSE(result.lines.back().is_justified);
}

TEST_F(TextLayoutTest, JustifySingleWord){
    auto result = layout.layout("Hello", 100.0f, TextAlign::Justify);
    ASSERT_EQ(result.lines.size(),1);
    EXPECT_EQ(result.lines[0].word_spacing, 10.0f);
}