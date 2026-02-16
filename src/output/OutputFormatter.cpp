#include "output/OutputFormatter.h"
#include <iostream>
#include <sstream>
#include <iomanip>

void OutputFormatter::printConsole(const LayoutNode &root)
{
    printConsoleRecursive(root, 0);
}

void OutputFormatter::printConsoleRecursive(const LayoutNode &node, int indent_level)
{

    std::cout << indent(indent_level);

    std::cout << node.getId() << ": "
              << "x=" << formatFloat(node.getComputedX()) << ", "
              << "y=" << formatFloat(node.getComputedY()) << ", "
              << "w=" << formatFloat(node.getComputedWidth()) << ", "
              << "h=" << formatFloat(node.getComputedHeight())
              << std::endl;

    for (size_t i = 0; i < node.getChildCount(); ++i)
    {
        printConsoleRecursive(node.getChild(i), indent_level + 1);
    }
}

std::string OutputFormatter::toJSON(const LayoutNode &root, bool pretty)
{
    return toJSONRecursive(root, 0, pretty);
}

std::string OutputFormatter::toJSONRecursive(const LayoutNode &node, int indent_level, bool pretty)
{
    std::ostringstream json;

    std::string ind = pretty ? indent(indent_level) : "";
    std::string ind_next = pretty ? indent(indent_level + 1) : "";
    std::string newline = pretty ? "\n" : "";
    std::string space = pretty ? " " : "";

    json << "{" << newline;

    // Node properties
    json << ind_next << "\"id\":" << space << "\"" << node.getId() << "\"," << newline;
    json << ind_next << "\"x\":" << space << formatFloat(node.getComputedX()) << "," << newline;
    json << ind_next << "\"y\":" << space << formatFloat(node.getComputedY()) << "," << newline;
    json << ind_next << "\"width\":" << space << formatFloat(node.getComputedWidth()) << "," << newline;
    json << ind_next << "\"height\":" << space << formatFloat(node.getComputedHeight());

    if (node.getChildCount() > 0)
    {
        json << "," << newline;
        json << ind_next << "\"children\":" << space << "[" << newline;

        for (size_t i = 0; i < node.getChildCount(); i++)
        {
            // indent child json
            std::string child_json = toJSONRecursive(node.getChild(i), indent_level + 2, pretty);

            if (pretty)
            {
                json << indent(indent_level + 2);
            }
            json << child_json;

            // add comma if not last child
            if (i < node.getChildCount() - 1)
            {
                json << ",";
            }
            json << newline;
        }
        json << ind_next << "]" << newline;
    }
    else
    {
        // empty children array
        json << "," << newline;
        json << ind_next << "\"children\":" << space << "[]" << newline;
    }

    // closing brace
    json << ind << "}";

    return json.str();
}

std::string OutputFormatter::indent(int level, int spaces_per_level)
{
    return std::string(level * spaces_per_level, ' ');
}

std::string OutputFormatter::formatFloat(float value, int precision)
{
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(precision) << value;
    return ss.str();
}