#ifndef BLOCK_LAYOUT_H
#define BLOCK_LAYOUT_H

#include "core/LayoutNode.h"
#include "core/layout/LayoutConstraints.h"

// BlockLayout namespace contains the CSS block layout algorithm
// Block layout stacks children vertically (like <div> elements in HTML)
namespace BlockLayout {
    void layout(LayoutNode& node, const LayoutConstraints& constraints);

} 

#endif
