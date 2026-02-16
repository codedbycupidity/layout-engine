#include "parser/LayoutParser.h"
#include "layout/LayoutEngine.h"
#include "output/OutputFormatter.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    // Check command line arguments
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> [options]" << std::endl;
        std::cerr << "Options:" << std::endl;
        std::cerr << "  --viewport <width> <height>  Set viewport dimensions (default: 1024x768)" << std::endl;
        std::cerr << "  --json                       Output in JSON format" << std::endl;
        return 1;
    }

    // Parse command line arguments
    std::string input_filename = argv[1];
    float viewport_width = 1024.0f;
    float viewport_height = 768.0f;
    bool json_output = false;

    // Parse optional arguments
    for (int i = 2; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "--viewport" && i + 2 < argc)
        {
            try
            {
                viewport_width = std::stof(argv[i + 1]);
                viewport_height = std::stof(argv[i + 2]);
                i += 2;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error: Invalid viewport dimensions" << std::endl;
                return 1;
            }
        }
        else if (arg == "--json")
        {
            json_output = true;
        }
        else
        {
            std::cerr << "Warning: Unknown option " << arg << std::endl;
        }
    }

    // Step 1: Parse input file
    LayoutParser parser;
    std::unique_ptr<LayoutNode> root;

    try
    {
        root = parser.parseFile(input_filename);
    }
    catch (const ParseError &e)
    {
        std::cerr << "Parse error: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error reading file: " << e.what() << std::endl;
        return 1;
    }

    if (!root)
    {
        std::cerr << "Error: Failed to parse input file" << std::endl;
        return 1;
    }

    // Step 2: Run layout
    LayoutEngine engine;
    engine.layout(*root, viewport_width, viewport_height);

    // Step 3: Output results
    if (json_output)
    {
        std::cout << OutputFormatter::toJSON(*root) << std::endl;
    }
    else
    {
        std::cout << "\n=== Layout Results ===" << std::endl;
        std::cout << "Viewport: " << viewport_width << "x" << viewport_height << std::endl;
        std::cout << "\n";
        OutputFormatter::printConsole(*root);
        std::cout << std::endl;
    }

    return 0;
}
