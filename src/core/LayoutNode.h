#ifndef LAYOUT_NODE_H
#define LAYOUT_NODE_H

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include "core/EdgeValues.h"
#include "core/LayoutEnums.h"

//LayoutNode -> represents a single box in the layout tree

class LayoutNode{
    public:
    explicit LayoutNode(std::string id);

    //destructor
    ~LayoutNode() = default;

    //disable copy constructor/assignment OP -> nodes shouldn't be copied, only moved
    LayoutNode(const LayoutNode&) = delete;
    LayoutNode& operator=(const LayoutNode&) = delete;

    //enable move constructor/assignment OP -> allow transferring ownership
    LayoutNode(LayoutNode&&) = default;
    LayoutNode& operator=(LayoutNode&&) = default;

    //property setters
    void setDisplay(Display display);
    void setWidth(float width);
    void setHeight(float height);
    void setMargin(const EdgeValues& margin);
    void setPadding(const EdgeValues& padding);
    void setBorder(const EdgeValues& border);
    void setFlexDirection(FlexDirection direction);
    void setJustifyContent(JustifyContent justify);
    void setAlignItems(AlignItems align);

    //property getters (const - dont modify node)
    const std::string& getId() const { return m_id; }
    Display getDisplay() const { return m_display; }
    std::optional<float> getWidth() const {return m_width;}
    std::optional<float> getHeight() const {return m_height;}
    const EdgeValues& getMargin() const {return m_margin;}
    const EdgeValues& getPadding() const {return m_padding;}
    const EdgeValues& getBorder() const {return m_border;}
    FlexDirection getFlexDirection() const {return m_flex_direction;}
    JustifyContent getJustifyContent() const {return m_justify_content;}
    AlignItems getAlignItems() const {return m_align_items;}

    //computed value setters/getters
    void setComputedPosition(float x, float y);
    void setComputedSize(float width, float height);

    float getComputedX() const {return m_computed_x;}
    float getComputedY() const {return m_computed_y;}
    float getComputedWidth() const {return m_computed_width;}
    float getComputedHeight() const {return m_computed_height;}

    //tree manipulation
    void addChild(std::unique_ptr<LayoutNode> child);
    LayoutNode* getParent() const {return m_parent;}
    const LayoutNode& getChild(size_t index) const;
    LayoutNode& getChild(size_t index);
    size_t getChildCount() const {return m_children.size();}

    //helper methods
    bool hasExplicitWidth() const {return m_width.has_value();}
    bool hasExplicitHeight() const {return m_height.has_value();}
    float getTotalHorizontalSpace() const;
    float getTotalVerticalSpace() const;

private:
    //member variables
    // PROPERTIES (immutable after tree construction)
    std::string m_id;                           // Identifier for debugging
    Display m_display;                          // display: block | flex | none
    std::optional<float> m_width;               // width (nullopt = auto)
    std::optional<float> m_height;              // height (nullopt = auto)
    EdgeValues m_margin;                        // margin: top right bottom left
    EdgeValues m_padding;                       // padding: top right bottom left
    EdgeValues m_border;                        // border-width: top right bottom left
    FlexDirection m_flex_direction;             // flex-direction: row | column
    JustifyContent m_justify_content;           // justify-content alignment
    AlignItems m_align_items;                   // align-items alignment

    // COMPUTED VALUES (mutable during layout calculation)
    float m_computed_x;         // Final x position
    float m_computed_y;         // Final y position
    float m_computed_width;     // Final width
    float m_computed_height;    // Final height

    //tree structure
    LayoutNode* m_parent;
    std::vector<std::unique_ptr<LayoutNode>> m_children;

};
#endif