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

std::string OutputFormatter::textLayoutToJSON(const TextLayoutResult &result,
                                               float container_width,
                                               const std::string &text,
                                               const std::string &align,
                                               const FontMetrics &metrics)
{
    std::ostringstream json;

    json << "{\n";
    json << "  \"text\": \"" << text << "\",\n";
    json << "  \"align\": \"" << align << "\",\n";
    json << "  \"container_width\": " << formatFloat(container_width) << ",\n";
    json << "  \"char_width\": " << formatFloat(metrics.getCharWidth()) << ",\n";
    json << "  \"line_height\": " << formatFloat(metrics.getLineHeight()) << ",\n";
    json << "  \"letter_spacing\": " << formatFloat(metrics.getLetterSpacing()) << ",\n";
    json << "  \"total_width\": " << formatFloat(result.total_width) << ",\n";
    json << "  \"total_height\": " << formatFloat(result.total_height) << ",\n";
    json << "  \"lines\": [\n";

    for (size_t i = 0; i < result.lines.size(); ++i)
    {
        const auto &line = result.lines[i];
        json << "    {\"words\": [";
        for (size_t j = 0; j < line.words.size(); ++j)
        {
            json << "\"" << line.words[j] << "\"";
            if (j < line.words.size() - 1) json << ", ";
        }
        json << "], ";
        json << "\"width\": " << formatFloat(line.width) << ", ";
        json << "\"x_offset\": " << formatFloat(line.x_offset) << ", ";
        json << "\"y_position\": " << formatFloat(line.y_position) << ", ";
        json << "\"word_spacing\": " << formatFloat(line.word_spacing) << ", ";
        json << "\"is_justified\": " << (line.is_justified ? "true" : "false");
        json << "}";
        if (i < result.lines.size() - 1) json << ",";
        json << "\n";
    }

    json << "  ]\n}";
    return json.str();
}