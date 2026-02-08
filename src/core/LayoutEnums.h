#ifndef LAYOUT_ENUMS_H
#define LAYOUT_ENUMS_H

//how element partcipates in layout
enum class Display{
    Block, //block-level box -> stacks vertically, takes full width
    Flex, // flex container -> enables flexbox layout
    None //not rendered -> removes from layout tree
};

//main axis of flex container
enum class FlexDirection{
    Row, //main axis is horizontal
    Column //main axis is vertical
};

//alignment along main access
enum class JustifyContent {
    FlexStart, //items packed at start of main axis
    Center, //items centered on main axis
    FlexEnd, //items packed at end of main access
    SpaceBetween //items evenly distributed, first at start, last at end
};

enum class AlignItems {
    FlexStart, //items aligned at start of cross axis
    Center, //items centered on cross axis
    FlexEnd, //items aligned at end of cross axis
    Stretch //items stretched to fill cross axis (default)
};

#endif