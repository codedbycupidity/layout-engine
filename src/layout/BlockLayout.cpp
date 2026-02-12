#include "layout/BlockLayout.h"
#include "layout/FlexLayout.h"
#include <algorithm>

namespace BlockLayout
{

    void layout(LayoutNode &node, const LayoutConstraints &constraints)
    {
        // Step 1: Calculate container's content width
        float content_width;

        if (node.hasExplicitWidth())
        {
            content_width = node.getWidth().value();
        }
        else
        {
            content_width = constraints.available_width - node.getTotalHorizontalSpace();
        }

        content_width = std::max(0.0f, content_width);

        // Step 2: Layout children vertically
        float y_offset = 0.0f;

        for (size_t i = 0; i < node.getChildCount(); ++i)
        {
            LayoutNode &child = node.getChild(i);

            // Skip children with display: none
            if (child.getDisplay() == Display::None)
            {
                continue;
            }

            // Create constraints for child
            float child_available_height;
            bool is_height_definite;

            if (constraints.is_height_definite)
            {
                child_available_height = std::max(0.0f, constraints.available_height - y_offset);
                is_height_definite = true;
            }
            else
            {
                child_available_height = 10000.0f;
                is_height_definite = false;
            }

            LayoutConstraints child_constraints(
                content_width,
                child_available_height,
                true,
                is_height_definite);

            // Recursively layout child based on its display type
            if (child.getDisplay() == Display::Block)
            {
                BlockLayout::layout(child, child_constraints);
            }
            else if (child.getDisplay() == Display::Flex)
            {
                FlexLayout::layout(child, child_constraints);
            }

            // Calculate child's X position
            float child_x = node.getPadding().left() + node.getBorder().left() + child.getMargin().left();

            // Calculate child's Y position
            float child_y = node.getPadding().top() + node.getBorder().top() + y_offset + child.getMargin().top();

            // Set child's computed position
            child.setComputedPosition(child_x, child_y);

            // Update y_offset for next child
            y_offset += child.getMargin().top() +
                        child.getComputedHeight() +
                        child.getMargin().bottom();
        }

        // Step 3: Calculate container height
        float content_height;

        if (node.hasExplicitHeight())
        {
            content_height = node.getHeight().value();
        }
        else
        {
            content_height = y_offset;
        }

        content_height = std::max(0.0f, content_height);

        // Step 4: Set computed dimensions on container
        float total_width = content_width +
                            node.getPadding().horizontal() +
                            node.getBorder().horizontal();

        float total_height = content_height +
                             node.getPadding().vertical() +
                             node.getBorder().vertical();

        node.setComputedSize(total_width, total_height);
    }

} // namespace BlockLayout
