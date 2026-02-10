#include <gtest/gtest.h>
#include "layout/LayoutConstraints.h"

TEST(LayoutConstraintsTest, DefaultConstruction){
    LayoutConstraints constraints (800.0f, 600.0f);

    EXPECT_FLOAT_EQ(constraints.available_width, 800.0f);
    EXPECT_FLOAT_EQ(constraints.available_height, 600.0f);

     // Default is definite (both true)
    EXPECT_TRUE(constraints.is_width_definite);
    EXPECT_TRUE(constraints.is_height_definite);

    // Helper methods
    EXPECT_FALSE(constraints.hasInfiniteWidth());
    EXPECT_FALSE(constraints.hasInfiniteHeight());
}

TEST(LayoutConstraintsTest, IndefiniteWidth) {
    // Large value for width, but marked as indefinite
    LayoutConstraints constraints(10000.0f, 600.0f, false, true);
    //                                                ^^^^^ width is indefinite

    EXPECT_FLOAT_EQ(constraints.available_width, 10000.0f);
    EXPECT_FLOAT_EQ(constraints.available_height, 600.0f);

    EXPECT_FALSE(constraints.is_width_definite);   // Width is indefinite
    EXPECT_TRUE(constraints.is_height_definite);   // Height is definite

    // Helper methods
    EXPECT_TRUE(constraints.hasInfiniteWidth());   // Width is infinite
    EXPECT_FALSE(constraints.hasInfiniteHeight()); // Height is finite
}

TEST(LayoutConstraintsTest, IndefiniteHeight) {
    LayoutConstraints constraints(800.0f, 10000.0f, true, false);
    //                                                      ^^^^^ height is indefinite

    EXPECT_FLOAT_EQ(constraints.available_width, 800.0f);
    EXPECT_FLOAT_EQ(constraints.available_height, 10000.0f);

    EXPECT_TRUE(constraints.is_width_definite);    // Width is definite
    EXPECT_FALSE(constraints.is_height_definite);  // Height is indefinite

    // Helper methods
    EXPECT_FALSE(constraints.hasInfiniteWidth());  // Width is finite
    EXPECT_TRUE(constraints.hasInfiniteHeight());  // Height is infinite
}

TEST(LayoutConstraintsTest, BothIndefinite) {
    LayoutConstraints constraints(10000.0f, 10000.0f, false, false);

    EXPECT_FALSE(constraints.is_width_definite);
    EXPECT_FALSE(constraints.is_height_definite);

    EXPECT_TRUE(constraints.hasInfiniteWidth());
    EXPECT_TRUE(constraints.hasInfiniteHeight());
}

TEST(LayoutConstraintsTest, ShrinkMethod) {
    // Parent: 800px × 600px
    LayoutConstraints parent_constraints(800.0f, 600.0f);

    // Parent has padding: 10px all sides (20px horizontal, 20px vertical)
    // Parent has border: 5px all sides (10px horizontal, 10px vertical)
    // Total: 30px horizontal, 30px vertical
    float horizontal_space = 20.0f + 10.0f;  // 30px
    float vertical_space = 20.0f + 10.0f;    // 30px

    // Create child constraints
    LayoutConstraints child_constraints = parent_constraints.shrink(
        horizontal_space,
        vertical_space
    );

    // Child should have reduced space
    EXPECT_FLOAT_EQ(child_constraints.available_width, 770.0f);   // 800 - 30
    EXPECT_FLOAT_EQ(child_constraints.available_height, 570.0f);  // 600 - 30

    // Definiteness should be preserved
    EXPECT_TRUE(child_constraints.is_width_definite);
    EXPECT_TRUE(child_constraints.is_height_definite);
}

TEST(LayoutConstraintsTest, ShrinkPreservesIndefiniteness) {
    // Parent with indefinite width
    LayoutConstraints parent_constraints(10000.0f, 600.0f, false, true);

    LayoutConstraints child_constraints = parent_constraints.shrink(50.0f, 30.0f);

    // Child should still have indefinite width
    EXPECT_FALSE(child_constraints.is_width_definite);
    EXPECT_TRUE(child_constraints.is_height_definite);

    // Dimensions are shrunk
    EXPECT_FLOAT_EQ(child_constraints.available_width, 9950.0f);   // 10000 - 50
    EXPECT_FLOAT_EQ(child_constraints.available_height, 570.0f);   // 600 - 30
}

TEST(LayoutConstraintsTest, ZeroDimensions) {
    LayoutConstraints constraints(0.0f, 0.0f);

    EXPECT_FLOAT_EQ(constraints.available_width, 0.0f);
    EXPECT_FLOAT_EQ(constraints.available_height, 0.0f);
    EXPECT_TRUE(constraints.is_width_definite);
    EXPECT_TRUE(constraints.is_height_definite);
}

TEST(LayoutConstraintsTest, FractionalPixels) {
    LayoutConstraints constraints(799.5f, 600.75f);

    EXPECT_FLOAT_EQ(constraints.available_width, 799.5f);
    EXPECT_FLOAT_EQ(constraints.available_height, 600.75f);
}