#ifndef LAYOUT_PARSER_H
#define LAYOUT_PARSER_H

#include "core/LayoutNode.h"
#include <string>
#include <memory>
#include <stdexcept>
// custom exception for parse errors
class ParseError : public std::runtime_error
{
public:
    ParseError(const std::string &message, int line_number)
        : std::runtime_error("Parse error at line " + std::to_string(line_number) + ":" + message),
          m_line_number(line_number)
    {
    }
    int getLineNumber() const { return m_line_number; }

private:
    int m_line_number;
};

// LayoutParser class parses text files into LayoutNode trees
class LayoutParser
{
public:
    // parse a layout file and return root node
    std::unique_ptr<LayoutNode> parseFile(const std::string &filename);

    // parse from a string
    std::unique_ptr<LayoutNode> parseString(const std::string &content);

private:
    void parseProperty(LayoutNode &node, const std::string &key, const std::string &value);
    EdgeValues parseEdgeValues(const std::string &value);
    float parseFloat(const std::string &str);
    std::string trim(const std::string &str);
    std::vector<std::string> split(const std::string &str, char delimiter);

    // track line number for error reporting
    int m_current_line;
};

#endif