#ifndef LAYOUT_ENGINE_H
#define LAYOUT_ENGINE_H

#include "core/LayoutNode.h"
#include "layout/LayoutConstraints.h"

class LayoutEngine{
    public:
    //performs complete layout
    void layout(LayoutNode &root, float viewport_width, float viewport_height);

    private:
    //internal recursive layout dispatcher
    //delegates to appropriate layout algorithm based on display type
    void layoutNode(LayoutNode &node, const LayoutConstraints &constraints);
};

#endif