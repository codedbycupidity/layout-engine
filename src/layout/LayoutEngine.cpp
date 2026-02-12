#include "layout/LayoutEngine.h"
#include "layout/BlockLayout.h"
#include "layout/FlexLayout.h"

void LayoutEngine::layout(LayoutNode &root, float viewport_width, float viewport_height){
    root.setComputedPosition(0.0f,0.0f);

    if(!root.hasExplicitWidth()){
        root.setWidth(viewport_width);
    }

    if(!root.hasExplicitHeight()){
        root.setHeight(viewport_height);
    }

    LayoutConstraints constraints(
        viewport_width,
        viewport_height,
        true, //width is definite
        true //height is definite
    );

    //begin recursive layout from root
    layoutNode(root, constraints);
}
void LayoutEngine::layoutNode(LayoutNode &node, const LayoutConstraints &constraints){
    switch(node.getDisplay()){
        case Display::Block:
        //block layout handles vertical stacking of children
        BlockLayout::layout(node, constraints);
        break;

        case Display::Flex:
        //flex layout handles main/cross axis distribution
        FlexLayout::layout(node, constraints);
        break;

        case Display::None:
        //set size to zero and dont layout children
        node.setComputedSize(0.0f,0.0f);
        break;
        
        default:
        //for unknown types treat as block
        BlockLayout::layout(node, constraints);
        break;
    }
}
