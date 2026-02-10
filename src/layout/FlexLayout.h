#ifndef FLEX_LAYOUT_H
#define FLEX_LAYOUT_H

#include "core/LayoutNode.h"
#include "layout/LayoutConstraints.h"

// FlexLayout namespace contains the CSS flexbox layout algorithm
// Flexbox layout distributes children along a main axis (row or column)
namespace FlexLayout {
    void layout(LayoutNode& node, const LayoutConstraints& constraints);
}

#endif
