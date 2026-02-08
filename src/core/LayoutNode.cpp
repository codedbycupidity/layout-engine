#include "core/LayoutNode.h"
#include <stdexcept>

// constructor
LayoutNode::LayoutNode(std::string id)
    : m_id(std::move(id))                        // Move string to avoid copy
    , m_display(Display::Block)                  // Default to block layout
    , m_width(std::nullopt)                      // Auto width by default
    , m_height(std::nullopt)                     // Auto height by default
    , m_margin()                                 // Default EdgeValues (all zeros)
    , m_padding()                                // Default EdgeValues (all zeros)
    , m_border()                                 // Default EdgeValues (all zeros)
    , m_flex_direction(FlexDirection::Row)       // Horizontal flexbox by default
    , m_justify_content(JustifyContent::FlexStart)  // Items at start
    , m_align_items(AlignItems::Stretch)         // Stretch to fill
    , m_computed_x(0.0f)                         // Start at origin
    , m_computed_y(0.0f)                         // Start at origin
    , m_computed_width(0.0f)                     // Will be computed
    , m_computed_height(0.0f)                    // Will be computed
    , m_parent(nullptr)                          // No parent yet
    , m_children()                               // Empty children vector
{
}

// property setters
void LayoutNode::setDisplay(Display display)
{
    m_display = display;
}

void LayoutNode::setWidth(float width)
{
    m_width = width;
}

void LayoutNode::setHeight(float height)
{
    m_height = height;
}

void LayoutNode::setMargin(const EdgeValues &margin)
{
    // Copy EdgeValues object
    m_margin = margin;
}

void LayoutNode::setPadding(const EdgeValues &padding)
{
    m_padding = padding;
}

void LayoutNode::setBorder(const EdgeValues &border)
{
    m_border = border;
}

void LayoutNode::setFlexDirection(FlexDirection direction)
{
    m_flex_direction = direction;
}

void LayoutNode::setJustifyContent(JustifyContent justify)
{
    m_justify_content = justify;
}

void LayoutNode::setAlignItems(AlignItems align)
{
    m_align_items = align;
}

// computed value setters
void LayoutNode::setComputedPosition(float x, float y)
{
    m_computed_x = x;
    m_computed_y = y;
}

void LayoutNode::setComputedSize(float width, float height)
{
    m_computed_height = height;
    m_computed_width = width;
}

// tree manipulation
void LayoutNode::addChild(std::unique_ptr<LayoutNode> child)
{
    LayoutNode *child_raw = child.get();
    child_raw->m_parent = this;

    // transfer ownership
    m_children.push_back(std::move(child));
}

const LayoutNode &LayoutNode::getChild(size_t index) const
{
    // dereferences to get reference
    return *m_children.at(index).get();
}

LayoutNode &LayoutNode::getChild(size_t index)
{
    // dereferences to get reference
    return *m_children.at(index).get();
}

// helper methods
float LayoutNode::getTotalHorizontalSpace() const
{
    return m_margin.horizontal() + m_border.horizontal() + m_padding.horizontal();
}

float LayoutNode::getTotalVerticalSpace() const
{
    return m_margin.vertical() + m_border.vertical() + m_padding.vertical();
}
