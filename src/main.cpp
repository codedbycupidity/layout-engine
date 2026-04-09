#include "parser/LayoutParser.h"
#include "layout/LayoutEngine.h"
#include "output/OutputFormatter.h"
#include "text/TextLayout.h"
#include "text/FontMetrics.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    // Check command line arguments
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> [options]" << std::endl;
        std::cerr << "       " << argv[0] << " --text-layout \"text\" [options]" << std::endl;
        std::cerr << "\nLayout mode options:" << std::endl;
        std::cerr << "  --viewport <width> <height>  Set viewport dimensions (default: 1024x768)" << std::endl;
        std::cerr << "  --json                       Output in JSON format" << std::endl;
        std::cerr << "\nText layout mode options:" << std::endl;
        std::cerr << "  --text-layout \"text\"          Run text layout engine" << std::endl;
        std::cerr << "  --width <px>                 Container width (default: 200)" << std::endl;
        std::cerr << "  --align <mode>               left|center|right|justify (default: left)" << std::endl;
        std::cerr << "  --char-width <px>            Character width (default: 8)" << std::endl;
        std::cerr << "  --line-height <px>           Line height (default: 20)" << std::endl;
        std::cerr << "  --letter-spacing <px>        Letter spacing (default: 0)" << std::endl;
        return 1;
    }

    // Check if text layout mode
    std::string first_arg = argv[1];
    if (first_arg == "--text-layout")
    {
        if (argc < 3)
        {
            std::cerr << "Error: --text-layout requires a text string" << std::endl;
            return 1;
        }

        std::string text = argv[2];
        //defaults
        float container_width = 200.0f;
        float char_width = 8.0f;
        float line_height = 20.0f;
        float letter_spacing = 0.0f;
        std::string align_str = "left";

        for (int i = 3; i < argc; ++i)
        {
            std::string arg = argv[i];
            try
            {
                if (arg == "--width" && i + 1 < argc)
                    container_width = std::stof(argv[++i]);
                else if (arg == "--align" && i + 1 < argc)
                    align_str = argv[++i];
                else if (arg == "--char-width" && i + 1 < argc)
                    char_width = std::stof(argv[++i]);
                else if (arg == "--line-height" && i + 1 < argc)
                    line_height = std::stof(argv[++i]);
                else if (arg == "--letter-spacing" && i + 1 < argc)
                    letter_spacing = std::stof(argv[++i]);
                else
                    std::cerr << "Warning: Unknown option " << arg << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error: Invalid value for " << arg << std::endl;
                return 1;
            }
        }

        //more defaults
        TextAlign align = TextAlign::Left;
        if (align_str == "center") align = TextAlign::Center;
        else if (align_str == "right") align = TextAlign::Right;
        else if (align_str == "justify") align = TextAlign::Justify;

        FontMetrics metrics(char_width, line_height, letter_spacing);
        TextLayout text_layout(metrics);
        TextLayoutResult result = text_layout.layout(text, container_width, align);

        std::cout << OutputFormatter::textLayoutToJSON(result, container_width, text, align_str, metrics) << std::endl;
        return 0;
    }

    // --- Original layout mode below ---

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
