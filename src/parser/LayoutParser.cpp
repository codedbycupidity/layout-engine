#include "parser/LayoutParser.h"
#include <fstream>
#include <sstream>
#include <stack>
#include <algorithm>
#include <cctype>

// parse from file
std::unique_ptr<LayoutNode> LayoutParser::parseFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw ParseError("Could not open file: " + filename, 0);
    }

    // read file into string
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return parseString(buffer.str());
}

// parse from string
// using a stack to track parent nodes, parse line by line
// cases:  node start "{", node end "}", property "key: value"
// build tree as we go"

std::unique_ptr<LayoutNode> LayoutParser::parseString(const std::string &content)
{
    std::unique_ptr<LayoutNode> root;
    std::stack<LayoutNode *> parent_stack;
    LayoutNode *current_parent = nullptr;

    m_current_line = 0;

    // split content into lines
    std::istringstream stream(content);
    std::string line;

    while (std::getline(stream, line))
    {
        m_current_line++;

        // trim whitespace
        line = trim(line);

        // skip empty lines and comments
        if (line.empty() || line[0] == '#' || line.substr(0, 2) == "//")
        {
            continue;
        }

        // Case 1: Node start (contains "{")
        if (line.find('{') != std::string::npos)
        {

            // extract node ID
            size_t brace_pos = line.find('{');
            std::string node_id = trim(line.substr(0, brace_pos));

            if (node_id.empty())
            {
                throw ParseError("node must have an identifier", m_current_line);
            }

            auto new_node = std::make_unique<LayoutNode>(node_id);

            if (!root)
            {
                // first node becomes root
                current_parent = new_node.get();
                parent_stack.push(current_parent);
                root = std::move(new_node);
            }
            else
            {
                if (!current_parent)
                {
                    throw ParseError("unexpected node (no parent context)", m_current_line);
                }

                LayoutNode *node_ptr = new_node.get();
                current_parent->addChild(std::move(new_node));
                parent_stack.push(node_ptr);
                current_parent = node_ptr;
            }
        }

        else if (line.find('}') != std::string::npos)
        {
            if (parent_stack.empty())
            {
                throw ParseError("unexpected '}' no matching '{'", m_current_line);
            }

            parent_stack.pop();

            if (!parent_stack.empty())
            {
                current_parent = parent_stack.top();
            }
            else
            {
                current_parent = nullptr;
            }
        }

        else if (line.find(':') != std::string::npos)
        {
            if (!current_parent)
            {
                throw ParseError("property outside of node", m_current_line);
            }

            // split by ":"
            size_t colon_pos = line.find(':');
            std::string key = trim(line.substr(0, colon_pos));
            std::string value = trim(line.substr(colon_pos+1));

            //remove trailing semicolon if present
            if(!value.empty() && value.back() == ';'){
                value = value.substr(0, value.length() - 1);
                value = trim(value);
            }

            parseProperty(*current_parent, key, value);
        }
        else
        {
            throw ParseError("unrecgnized syntax", m_current_line);
        }
    }

    // check for unclosed nodes
    if (!parent_stack.empty())
    {
        throw ParseError("unclosed node (missing '}')", m_current_line);
    }

    return root;
}

// parse property and apply to node
void LayoutParser::parseProperty(LayoutNode &node, const std::string &key, const std::string &value)
{
    if (key == "display")
    {
        if (value == "block")
            node.setDisplay(Display::Block);
        else if (value == "flex")
            node.setDisplay(Display::Flex);
        else if (value == "none")
            node.setDisplay(Display::None);
        else
            throw ParseError("invalid display value: " + value, m_current_line);
    }
    else if (key == "width")
    {
        node.setWidth(parseFloat(value));
    }
    else if (key == "height")
    {
        node.setHeight(parseFloat(value));
    }
    else if (key == "margin")
    {
        node.setMargin(parseEdgeValues(value));
    }
    else if (key == "padding")
    {
        node.setPadding(parseEdgeValues(value));
    }
    else if (key == "border")
    {
        node.setBorder(parseEdgeValues(value));
    }
    else if (key == "flex-direction")
    {
        if (value == "row")
            node.setFlexDirection(FlexDirection::Row);
        else if (value == "column")
            node.setFlexDirection(FlexDirection::Column);
        else
            throw ParseError("invalid flex-direction value: " + value, m_current_line);
    }
    else if (key == "justify-content")
    {
        if (value == "flex-start")
            node.setJustifyContent(JustifyContent::FlexStart);
        else if (value == "center")
            node.setJustifyContent(JustifyContent::Center);
        else if (value == "flex-end")
            node.setJustifyContent(JustifyContent::FlexEnd);
        else if (value == "space-between")
            node.setJustifyContent(JustifyContent::SpaceBetween);
        else
            throw ParseError("Invalid justify-content value: " + value, m_current_line);
    }
    else if (key == "align-items")
    {
        if (value == "flex-start")
            node.setAlignItems(AlignItems::FlexStart);
        else if (value == "center")
            node.setAlignItems(AlignItems::Center);
        else if (value == "flex-end")
            node.setAlignItems(AlignItems::FlexEnd);
        else if (value == "stretch")
            node.setAlignItems(AlignItems::Stretch);
        else
            throw ParseError("Invalid align-items value: " + value, m_current_line);
    }
    else
    {
        // Unknown property - could warn or ignore
        // For now, throw to catch typos during development
        throw ParseError("Unknown property: " + key, m_current_line);
    }
}

// parge edge values (margin, padding, border)
EdgeValues LayoutParser::parseEdgeValues(const std::string &value)
{
    std::vector<std::string> tokens = split(value, ' ');

    // remove empty tokens
    tokens.erase(std::remove_if(tokens.begin(), tokens.end(),
                                [](const std::string &s)
                                { return s.empty(); }),
                 tokens.end());

    if (tokens.size() == 1)
    {
        // all sides same
        float val = parseFloat(tokens[0]);
        return EdgeValues(val, val, val, val);
    }
    else if (tokens.size() == 2)
    {
        // vertical horizontal
        float vertical = parseFloat(tokens[0]);
        float horizontal = parseFloat(tokens[1]);
        return EdgeValues(vertical, horizontal, vertical, horizontal);
    }
    else if (tokens.size() == 4)
    {
        // top right bottom left
        float top = parseFloat(tokens[0]);
        float right = parseFloat(tokens[1]);
        float bottom = parseFloat(tokens[2]);
        float left = parseFloat(tokens[3]);
        return EdgeValues(top, right, bottom, left);
    }
    else
    {
        throw ParseError("Invalid edge values format (expected 1, 2, or 4 values)", m_current_line);
    }
}

float LayoutParser::parseFloat(const std::string &str)
{
    try
    {
        return std::stof(str);
    }
    catch (const std::exception &)
    {
        throw ParseError("Invalid number format: " + str, m_current_line);
    }
}

// trim whitespace from string
std::string LayoutParser::trim(const std::string &str)
{
    size_t start = 0;
    size_t end = str.length();

    // Find first non-whitespace
    while (start < end && std::isspace(str[start]))
    {
        start++;
    }

    // Find last non-whitespace
    while (end > start && std::isspace(str[end - 1]))
    {
        end--;
    }

    return str.substr(start, end - start);
}

//split string by delimiter
std::vector<std::string> LayoutParser::split(const std::string &str, char delimiter){
    std::vector<std::string> tokens;
    std::istringstream stream(str);
    std::string token;

    while(std::getline(stream, token, delimiter)){
        tokens.push_back(token);
    }
    return tokens;
}