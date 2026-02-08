#ifndef LAYOUT_CONSTRAINTS_H
#define LAYOUT_CONSTRAINTS_H


struct LayoutConstraints{
    //member variables
    float available_width;
    float available_height;

    bool is_width_definite;
    bool is_height_definite;

    LayoutConstraints(
        float width,
        float height,
        bool width_definite = true,
        bool height_definite = true
    )
    : available_width(width)
    , available_height(height)
    , is_width_definite(width_definite)
    , is_height_definite(height_definite)
    {}

    //helper methods
    bool hasInfiniteWidth() const {
        return !is_width_definite;
    }
    bool hasInfiniteHeight() const {
        return !is_height_definite;
    }

    LayoutConstraints shrink(float horizontal_space, float vertical_space) const {
        return LayoutConstraints(
            available_width - horizontal_space,
            available_height - vertical_space,
            is_width_definite,
            is_height_definite
        );
    }
};

#endif