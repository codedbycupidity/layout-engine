#include "layout/FlexLayout.h"
#include "layout/BlockLayout.h"
#include <algorithm>
#include <vector>

namespace FlexLayout
{

    struct FlexItem
    {
        LayoutNode *node;
        float main_size;
        float cross_size;
    };

    float calculateCrossAxisPositionRow(
        const FlexItem &item,
        const LayoutNode &container,
        float container_cross_size)
    {
        AlignItems align = container.getAlignItems();

        float container_padding_top = container.getPadding().top();
        float container_border_top = container.getBorder().top();
        float item_margin_top = item.node->getMargin().top();

        switch (align)
        {
        case AlignItems::FlexStart:
            return container_padding_top + container_border_top + item_margin_top;

        case AlignItems::Center:
        {
            float available_space = container_cross_size - item.cross_size;
            return container_padding_top + container_border_top + (available_space / 2.0f) + item_margin_top;
        }

        case AlignItems::FlexEnd:
        {
            float available_space = container_cross_size - item.cross_size;
            return container_padding_top + container_border_top + available_space + item_margin_top;
        }

        case AlignItems::Stretch:
        {
            return container_padding_top + container_border_top + item_margin_top;
        }

        default:
            return container_padding_top + container_border_top + item_margin_top;
        }
    }

    float calculateCrossAxisPositionColumn(
        const FlexItem &item,
        const LayoutNode &container,
        float container_cross_size)
    {
        AlignItems align = container.getAlignItems();

        float container_padding_left = container.getPadding().left();
        float container_border_left = container.getBorder().left();
        float item_margin_left = item.node->getMargin().left();

        switch (align)
        {
        case AlignItems::FlexStart:
            return container_padding_left + container_border_left + item_margin_left;

        case AlignItems::Center:
        {
            float available_space = container_cross_size - item.cross_size;
            return container_padding_left + container_border_left +
                   (available_space / 2.0f) + item_margin_left;
        }

        case AlignItems::FlexEnd:
        {
            float available_space = container_cross_size - item.cross_size;
            return container_padding_left + container_border_left +
                   available_space + item_margin_left;
        }

        case AlignItems::Stretch:
        {
            return container_padding_left + container_border_left + item_margin_left;
        }

        default:
            return container_padding_left + container_border_left + item_margin_left;
        }
    }

    void layoutFlexRow(LayoutNode &node, const LayoutConstraints &constraints)
    {
        // Step 1: Calculate container content width
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

        // Step 2: Layout all flex items and measure them
        std::vector<FlexItem> items;
        float total_main_size = 0.0f;
        float max_cross_size = 0.0f;

        for (size_t i = 0; i < node.getChildCount(); i++)
        {
            LayoutNode &child = node.getChild(i);

            if (child.getDisplay() == Display::None)
            {
                continue;
            }

            LayoutConstraints child_constraints(
                content_width,
                constraints.available_height,
                true,
                constraints.is_height_definite);

            if (child.getDisplay() == Display::Block)
            {
                BlockLayout::layout(child, child_constraints);
            }
            else if (child.getDisplay() == Display::Flex)
            {
                FlexLayout::layout(child, child_constraints);
            }

            FlexItem item;
            item.node = &child;
            item.main_size = child.getComputedWidth() +
                             child.getMargin().left() +
                             child.getMargin().right();
            item.cross_size = child.getComputedHeight() +
                              child.getMargin().top() +
                              child.getMargin().bottom();

            items.push_back(item);
            total_main_size += item.main_size;
            max_cross_size = std::max(max_cross_size, item.cross_size);
        }

        // Step 2.5: Apply stretch if align-items is Stretch
        if (node.getAlignItems() == AlignItems::Stretch)
        {
            for (FlexItem &item : items)
            {
                if (!item.node->hasExplicitHeight())
                {
                    float stretched_height = max_cross_size -
                                             item.node->getMargin().top() -
                                             item.node->getMargin().bottom();

                    stretched_height -= item.node->getPadding().vertical() +
                                        item.node->getBorder().vertical();

                    stretched_height = std::max(0.0f, stretched_height);

                    float new_total_height = stretched_height +
                                             item.node->getPadding().vertical() +
                                             item.node->getBorder().vertical();

                    item.node->setComputedSize(item.node->getComputedWidth(), new_total_height);

                    item.cross_size = new_total_height +
                                      item.node->getMargin().top() +
                                      item.node->getMargin().bottom();
                }
            }
        }

        // Step 3: Calculate spacing based on justify-content
        float available_space = content_width - total_main_size;
        float current_x = 0.0f;
        float spacing_between_items = 0.0f;

        JustifyContent justify = node.getJustifyContent();

        switch (justify)
        {
        case JustifyContent::FlexStart:
            current_x = 0.0f;
            spacing_between_items = 0.0f;
            break;

        case JustifyContent::Center:
            current_x = std::max(0.0f, available_space / 2.0f);
            spacing_between_items = 0.0f;
            break;

        case JustifyContent::FlexEnd:
            current_x = std::max(0.0f, available_space);
            spacing_between_items = 0.0f;
            break;

        case JustifyContent::SpaceBetween:
            current_x = 0.0f;
            if (items.size() > 1)
            {
                spacing_between_items = std::max(0.0f, available_space / (items.size() - 1));
            }
            else
            {
                spacing_between_items = 0.0f;
            }
            break;

        default:
            current_x = 0.0f;
            spacing_between_items = 0.0f;
            break;
        }

        // Step 4: Position all items
        for (FlexItem &item : items)
        {
            float x = node.getPadding().left() +
                      node.getBorder().left() +
                      current_x +
                      item.node->getMargin().left();

            float y = calculateCrossAxisPositionRow(item, node, max_cross_size);

            item.node->setComputedPosition(x, y);

            current_x += item.main_size + spacing_between_items;
        }

        // Step 5: Calculate container size
        float container_width;
        if (node.hasExplicitWidth())
        {
            container_width = node.getWidth().value();
        }
        else
        {
            container_width = total_main_size;
        }

        float container_height;
        if (node.hasExplicitHeight())
        {
            container_height = node.getHeight().value();
        }
        else
        {
            container_height = max_cross_size;
        }

        float total_width = container_width + node.getTotalHorizontalSpace();
        float total_height = container_height + node.getTotalVerticalSpace();

        node.setComputedSize(total_width, total_height);
    }

    void layoutFlexColumn(LayoutNode &node, const LayoutConstraints &constraints)
    {
        // Step 1: Calculate container content height
        float content_height;
        if (node.hasExplicitHeight())
        {
            content_height = node.getHeight().value();
        }
        else
        {
            content_height = constraints.is_height_definite ? (constraints.available_height - node.getTotalVerticalSpace()) : 10000.0f;
        }
        content_height = std::max(0.0f, content_height);

        // Step 2: Layout all flex items and measure them
        std::vector<FlexItem> items;
        float total_main_size = 0.0f;
        float max_cross_size = 0.0f;

        for (size_t i = 0; i < node.getChildCount(); ++i)
        {
            LayoutNode &child = node.getChild(i);

            if (child.getDisplay() == Display::None)
            {
                continue;
            }

            LayoutConstraints child_constraints(
                constraints.available_width,
                content_height,
                constraints.is_width_definite,
                true);

            if (child.getDisplay() == Display::Block)
            {
                BlockLayout::layout(child, child_constraints);
            }
            else if (child.getDisplay() == Display::Flex)
            {
                FlexLayout::layout(child, child_constraints);
            }

            FlexItem item;
            item.node = &child;
            item.main_size = child.getComputedHeight() +
                             child.getMargin().top() +
                             child.getMargin().bottom();
            item.cross_size = child.getComputedWidth() +
                              child.getMargin().left() +
                              child.getMargin().right();

            items.push_back(item);
            total_main_size += item.main_size;
            max_cross_size = std::max(max_cross_size, item.cross_size);
        }

        // Step 2.5: Apply stretch if align-items is Stretch
        if (node.getAlignItems() == AlignItems::Stretch)
        {
            for (FlexItem &item : items)
            {
                if (!item.node->hasExplicitWidth())
                {
                    float stretched_width = max_cross_size -
                                            item.node->getMargin().left() -
                                            item.node->getMargin().right();

                    stretched_width -= item.node->getPadding().horizontal() +
                                       item.node->getBorder().horizontal();

                    stretched_width = std::max(0.0f, stretched_width);

                    float new_total_width = stretched_width +
                                            item.node->getPadding().horizontal() +
                                            item.node->getBorder().horizontal();

                    item.node->setComputedSize(new_total_width, item.node->getComputedHeight());

                    item.cross_size = new_total_width +
                                      item.node->getMargin().left() +
                                      item.node->getMargin().right();
                }
            }
        }

        // Step 3: Calculate spacing based on justify-content
        float container_content_height = node.hasExplicitHeight() ? node.getHeight().value() : total_main_size;
        float available_space = container_content_height - total_main_size;
        float current_y = 0.0f;
        float spacing_between_items = 0.0f;

        JustifyContent justify = node.getJustifyContent();

        switch (justify)
        {
        case JustifyContent::FlexStart:
            current_y = 0.0f;
            spacing_between_items = 0.0f;
            break;

        case JustifyContent::Center:
            current_y = std::max(0.0f, available_space / 2.0f);
            spacing_between_items = 0.0f;
            break;

        case JustifyContent::FlexEnd:
            current_y = std::max(0.0f, available_space);
            spacing_between_items = 0.0f;
            break;

        case JustifyContent::SpaceBetween:
            current_y = 0.0f;
            if (items.size() > 1)
            {
                spacing_between_items = std::max(0.0f, available_space / (items.size() - 1));
            }
            else
            {
                spacing_between_items = 0.0f;
            }
            break;

        default:
            current_y = 0.0f;
            spacing_between_items = 0.0f;
            break;
        }

        // Step 4: Position all items
        for (FlexItem &item : items)
        {
            float x = calculateCrossAxisPositionColumn(item, node, max_cross_size);

            float y = node.getPadding().top() +
                      node.getBorder().top() +
                      current_y +
                      item.node->getMargin().top();

            item.node->setComputedPosition(x, y);

            current_y += item.main_size + spacing_between_items;
        }

        // Step 5: Calculate container size
        float container_width;
        if (node.hasExplicitWidth())
        {
            container_width = node.getWidth().value();
        }
        else
        {
            container_width = max_cross_size;
        }

        float container_height;
        if (node.hasExplicitHeight())
        {
            container_height = node.getHeight().value();
        }
        else
        {
            container_height = total_main_size;
        }

        float total_width = container_width + node.getTotalHorizontalSpace();
        float total_height = container_height + node.getTotalVerticalSpace();

        node.setComputedSize(total_width, total_height);
    }

    void layout(LayoutNode &node, const LayoutConstraints &constraints)
    {
        FlexDirection direction = node.getFlexDirection();

        if (direction == FlexDirection::Row)
        {
            layoutFlexRow(node, constraints);
        }
        else
        {
            layoutFlexColumn(node, constraints);
        }
    }

} // namespace FlexLayout
