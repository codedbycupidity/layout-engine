#ifndef OUTPUT_FORMATTER_H
#define OUTPUT_FORMATTER_H

#include "core/LayoutNode.h"
#include <string>

class OutputFormatter
{
public:
    static void printConsole(const LayoutNode &root);
    static std::string toJSON(const LayoutNode &root, bool pretty = true);

private:
    static void printConsoleRecursive(const LayoutNode &node, int indent_level);

    static std::string toJSONRecursive(const LayoutNode &node, int indent_level, bool pretty);

    static std::string indent(int level, int space_per_level = 2);

    static std::string formatFloat(float value, int precision = 1);
};

#endif