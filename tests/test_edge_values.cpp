#include <gtest/gtest.h>
#include "core/EdgeValues.h"

TEST(EdgeValuesTest, DefaultConsturction){
    EdgeValues edges;

    EXPECT_FLOAT_EQ(edges.top(), 0.0f);      // Verify top = 0
    EXPECT_FLOAT_EQ(edges.right(), 0.0f);    // Verify right = 0
    EXPECT_FLOAT_EQ(edges.bottom(), 0.0f);   // Verify bottom = 0
    EXPECT_FLOAT_EQ(edges.left(), 0.0f);     // Verify left = 0

    // Test aggregate methods as well
    EXPECT_FLOAT_EQ(edges.horizontal(), 0.0f);  // 0 + 0 = 0
    EXPECT_FLOAT_EQ(edges.vertical(), 0.0f);    // 0 + 0 = 0
}

TEST(EdgeValuesTest, UniformConstruction){
    EdgeValues edges(10.0f);

    EXPECT_FLOAT_EQ(edges.top(), 10.0f);
    EXPECT_FLOAT_EQ(edges.right(), 10.0f);
    EXPECT_FLOAT_EQ(edges.bottom(), 10.0f);
    EXPECT_FLOAT_EQ(edges.left(), 10.0f);

    // horizontal() should return left + right = 10 + 10 = 20
    EXPECT_FLOAT_EQ(edges.horizontal(), 20.0f);
    // vertical() should return top + bottom = 10 + 10 = 20
    EXPECT_FLOAT_EQ(edges.vertical(), 20.0f);
}

TEST(EdgeValuesTest, IndividualConstruction) {
    // equivalent to CSS: margin: 5px 10px 15px 20px;
    //                            top  right bottom left
    EdgeValues edges(5.0f, 10.0f, 15.0f, 20.0f);

    EXPECT_FLOAT_EQ(edges.top(), 5.0f);       // First parameter
    EXPECT_FLOAT_EQ(edges.right(), 10.0f);    // Second parameter
    EXPECT_FLOAT_EQ(edges.bottom(), 15.0f);   // Third parameter
    EXPECT_FLOAT_EQ(edges.left(), 20.0f);     // Fourth parameter
}

TEST(EdgeValuesTest, HorizontalCalculation) {
    EdgeValues edges(5.0f, 10.0f, 15.0f, 20.0f);
    //               top   right  bottom left

    // ASSERT: Verify horizontal() returns left + right
    // Expected: 20.0 (left) + 10.0 (right) = 30.0
    EXPECT_FLOAT_EQ(edges.horizontal(), 30.0f);
}

TEST(EdgeValuesTest, VerticalCalculation) {
    EdgeValues edges(5.0f, 10.0f, 15.0f, 20.0f);
    //               top   right  bottom left

    // ASSERT: Verify vertical() returns top + bottom
    // Expected: 5.0 (top) + 15.0 (bottom) = 20.0
    EXPECT_FLOAT_EQ(edges.vertical(), 20.0f);
}

TEST(EdgeValuesTest, FloatingPointPrecision) {
    EdgeValues edges(1.5f, 2.25f, 3.75f, 4.5f);
    //               top   right  bottom left

    EXPECT_FLOAT_EQ(edges.top(), 1.5f);
    EXPECT_FLOAT_EQ(edges.right(), 2.25f);
    EXPECT_FLOAT_EQ(edges.bottom(), 3.75f);
    EXPECT_FLOAT_EQ(edges.left(), 4.5f);

    // ASSERT: Verify aggregate calculations with fractional arithmetic
    // horizontal() = 4.5 + 2.25 = 6.75
    EXPECT_FLOAT_EQ(edges.horizontal(), 6.75f);
    // vertical() = 1.5 + 3.75 = 5.25
    EXPECT_FLOAT_EQ(edges.vertical(), 5.25f);
}

TEST(EdgeValuesTest, ZeroUniformConstruction) {
    EdgeValues edges(0.0f);

    // ASSERT: Verify aggregate methods handle zeros correctly
    EXPECT_FLOAT_EQ(edges.horizontal(), 0.0f);  // 0 + 0 = 0
    EXPECT_FLOAT_EQ(edges.vertical(), 0.0f);    // 0 + 0 = 0

    EXPECT_FLOAT_EQ(edges.top(), 0.0f);
    EXPECT_FLOAT_EQ(edges.right(), 0.0f);
    EXPECT_FLOAT_EQ(edges.bottom(), 0.0f);
    EXPECT_FLOAT_EQ(edges.left(), 0.0f);
}