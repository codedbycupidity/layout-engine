#include "parser/LayoutParser.h"
#include <fstream>
#include <sstream>
#include <stack>
#include <algorithm>
#include <cctype>

// parse from file
// opens a file, reads entire contents into a string, then hands it to parseString()
// this way the actual parsing logic only lives in one place (parseString)
std::unique_ptr<LayoutNode> LayoutParser::parseFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw ParseError("Could not open file: " + filename, 0);
    }

    // read entire file into a stringstream buffer, then convert to string
    // rdbuf() gives us the raw file buffer — faster than reading line by line
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return parseString(buffer.str());
}

// parse from string — the core parsing logic
//
// How it works:
//   We read the input line by line. Each line is one of three things:
//     1. "nodeName {"  → create a new node, push it onto the stack
//     2. "}"           → done with current node, pop the stack
//     3. "key: value;" → set a property on the current node
//
// The stack tracks nesting. The top of the stack is always the
// "current parent" — the node we're inside of right now.
//
// Example input:
//   container {          ← create root, stack = [container]
//       width: 800;      ← set width on container
//       child {          ← create child of container, stack = [container, child]
//           height: 100; ← set height on child
//       }                ← pop, stack = [container]
//   }                    ← pop, stack = [] (done)

std::unique_ptr<LayoutNode> LayoutParser::parseString(const std::string &content)
{
    std::unique_ptr<LayoutNode> root;                // the final tree we return (starts null)
    std::stack<LayoutNode *> parent_stack;            // tracks nesting — top = current parent
    LayoutNode *current_parent = nullptr;             // shortcut to parent_stack.top()

    m_current_line = 0;                               // for error messages ("error on line 5")

    // treat the string like a file — read it line by line
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

            // create the node — unique_ptr owns it for now
            auto new_node = std::make_unique<LayoutNode>(node_id);

            if (!root)
            {
                // very first node in the file — this becomes the root of the tree
                // grab raw pointer before moving ownership to root
                current_parent = new_node.get();
                parent_stack.push(current_parent);
                root = std::move(new_node);    // root now owns it, new_node is empty
            }
            else
            {
                // not the first node — this is a child of whatever node we're inside
                if (!current_parent)
                {
                    throw ParseError("unexpected node (no parent context)", m_current_line);
                }

                // grab raw pointer before move (can't access new_node after move)
                LayoutNode *node_ptr = new_node.get();
                // hand ownership to the parent — parent's m_children vector now owns it
                current_parent->addChild(std::move(new_node));
                // this child is now the current parent (we're inside its {})
                parent_stack.push(node_ptr);
                current_parent = node_ptr;
            }
        }

        // Case 2: Node end — "}" means we're done with the current node
        // pop it off the stack and go back to its parent
        else if (line.find('}') != std::string::npos)
        {
            if (parent_stack.empty())
            {
                throw ParseError("unexpected '}' no matching '{'", m_current_line);
            }

            parent_stack.pop();  // remove current node from stack

            // if stack still has nodes, the new top is our parent
            // if stack is empty, we've closed the root — nothing left
            if (!parent_stack.empty())
            {
                current_parent = parent_stack.top();
            }
            else
            {
                current_parent = nullptr;
            }
        }

        // Case 3: Property — "key: value;" sets a property on the current node
        // e.g. "width: 800;" or "display: flex;"
        else if (line.find(':') != std::string::npos)
        {
            // properties must be inside a node's {} block
            if (!current_parent)
            {
                throw ParseError("property outside of node", m_current_line);
            }

            // split "width: 800;" into key="width" and value="800"
            size_t colon_pos = line.find(':');
            std::string key = trim(line.substr(0, colon_pos));         // everything before ':'
            std::string value = trim(line.substr(colon_pos+1));        // everything after ':'

            // remove trailing semicolon if present (semicolons are optional)
            if(!value.empty() && value.back() == ';'){
                value = value.substr(0, value.length() - 1);
                value = trim(value);
            }

            // apply this property to the current node
            // *current_parent dereferences the pointer to get a reference
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
// takes a key-value pair like ("width", "800") and calls the right setter
// on the node. basically a big lookup table: key → setter method
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

// parse edge values (margin, padding, border)
// follows the same CSS shorthand rules:
//   1 value:  "10"          → all four sides = 10
//   2 values: "10 20"       → top/bottom = 10, left/right = 20
//   4 values: "10 20 30 40" → top=10, right=20, bottom=30, left=40
EdgeValues LayoutParser::parseEdgeValues(const std::string &value)
{
    // split "10 20 30 40" into ["10", "20", "30", "40"]
    std::vector<std::string> tokens = split(value, ' ');

    // remove empty tokens (in case of extra spaces like "10  20")
    tokens.erase(std::remove_if(tokens.begin(), tokens.end(),
                                [](const std::string &s)
                                { return s.empty(); }),
                 tokens.end());

    if (tokens.size() == 1)
    {
        // "10" → all four sides get the same value
        float val = parseFloat(tokens[0]);
        return EdgeValues(val, val, val, val);
    }
    else if (tokens.size() == 2)
    {
        // "10 20" → vertical=10 (top+bottom), horizontal=20 (left+right)
        float vertical = parseFloat(tokens[0]);
        float horizontal = parseFloat(tokens[1]);
        return EdgeValues(vertical, horizontal, vertical, horizontal);
    }
    else if (tokens.size() == 4)
    {
        // "10 20 30 40" → top=10, right=20, bottom=30, left=40 (clockwise)
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

// convert a string to float — wraps std::stof with a better error message
float LayoutParser::parseFloat(const std::string &str)
{
    try
    {
        return std::stof(str);  // "800" → 800.0f
    }
    catch (const std::exception &)
    {
        throw ParseError("Invalid number format: " + str, m_current_line);
    }
}

// trim whitespace from both ends of a string
// "  hello world  " → "hello world"
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

// split string by delimiter character
// "10 20 30" split by ' ' → ["10", "20", "30"]
std::vector<std::string> LayoutParser::split(const std::string &str, char delimiter){
    std::vector<std::string> tokens;
    std::istringstream stream(str);
    std::string token;

    while(std::getline(stream, token, delimiter)){
        tokens.push_back(token);
    }
    return tokens;
}