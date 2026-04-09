# Mini Layout Engine

A C++ implementation of core layout algorithms demonstrating understanding of browser rendering fundamentals. Built as preparation for the WebKit Layout and Rendering team internship.

## Project Overview

This project implements a simplified layout engine in C++ that:
- Parses a simplified layout description format (CSS-like properties)
- Constructs a layout tree
- Implements fundamental layout algorithms (Block layout + Flexbox basics)
- Performs text layout with line breaking and alignment
- Calculates positions and dimensions
- Includes comprehensive testing

## Relevance to WebKit

The WebKit Layout and Rendering team works on the core engine that determines how web content appears on billions of Apple devices. This project demonstrates:

### 1. Understanding of Layout Fundamentals
- How layout trees are constructed
- How layout algorithms compute positions and dimensions
- How properties cascade through the tree

### 2. C++ Skills Relevant to WebKit
- Modern C++17 practices (RAII, smart pointers, const-correctness)
- Object-oriented design for complex systems
- Memory management in performance-critical code
- Clear, maintainable code structure

### 3. WebKit Core Values

From WebKit's project goals, this demonstrates:

**Standards Compliance**: Implements algorithms following W3C specifications (Flexbox spec)

**Performance Awareness**: Measures and optimizes layout calculations

**Hackability**: Clean code, clear algorithms, easy to understand

**Engineering over Science**: Proven technologies, practical implementation

**Stability**: Comprehensive regression testing

**Portability**: Clean abstractions, platform-independent core

## Core Components

### 1. Layout Tree Structure
- Node hierarchy (Box model)
- Property inheritance
- Tree traversal

### 2. Block Layout Algorithm
- Normal flow layout
- Box model calculations (margin, border, padding, content)
- Containing block logic

### 3. Flexbox Layout Algorithm (Simplified)
- Main axis / cross axis
- Flex item sizing
- Justification and alignment basics

### 4. Text Layout Engine
- Word wrapping and line breaking algorithm
- Text alignment (left, center, right, justify)
- Simple font metrics (character width, line height, letter spacing)
- Justified text with even word spacing distribution

### 5. Layout Engine
- Coordinates layout passes
- Handles dependencies
- Computes final positions

### 6. Testing Framework
- Unit tests for layout calculations
- Regression test suite
- Performance benchmarks

## Project Scope

### Included Features 

- **Box model**: margin, border, padding, content
- **Block layout**: Vertical stacking, width/height calculation
- **Basic flexbox**: flex-direction (row/column), justify-content, align-items
- **Text layout**: Word wrapping, line breaking, left/center/right/justify alignment
- **Font metrics**: Configurable character width, line height, letter spacing
- **Positioning**: Normal flow only (no absolute/fixed)
- **Properties**: Width, height, margin, padding, display, flex properties
- **Testing**: Comprehensive unit and integration tests

## Skills Demonstrated

### C++ Skills
- Class design and inheritance
- Smart pointers (unique_ptr, shared_ptr)
- RAII and resource management
- Const-correctness
- STL containers (vector, map)
- Modern C++17 features
- CMake build system

### Algorithm Skills
- Tree algorithms (traversal, recursion)
- Layout calculation algorithms
- Constraint solving (flex sizing)
- Performance optimization

### Software Engineering Skills
- Clean architecture
- Unit testing
- API design
- Documentation
- Version control (git)

## Tech Stack

**Language**: C++17

**Build System**: CMake

**Testing**: Google Test (gtest)

**Optional Visualizer**: HTML5 + JavaScript (vanilla, no frameworks)

**Development**: Any platform (macOS, Linux, Windows)

## Project Structure

```
mini-layout-engine/
├── README.md                     # This file
├── CMakeLists.txt                # Build configuration
├── src/
│   ├── core/
│   │   ├── LayoutNode.h          # Layout tree node
│   │   ├── LayoutNode.cpp
│   │   ├── EdgeValues.h          # Box model edges
│   │   └── EdgeValues.cpp
│   │
│   ├── layout/
│   │   ├── LayoutEngine.h        # Main coordinator
│   │   ├── LayoutEngine.cpp
│   │   ├── BlockLayout.h         # Block layout algorithm
│   │   ├── BlockLayout.cpp
│   │   ├── FlexLayout.h          # Flexbox algorithm
│   │   └── FlexLayout.cpp
│   │
│   ├── parser/
│   │   ├── LayoutParser.h        # Input parser
│   │   └── LayoutParser.cpp
│   │
│   ├── output/
│   │   ├── OutputFormatter.h     # Output formatting
│   │   └── OutputFormatter.cpp
│   │
│   ├── text/
│   │   ├── FontMetrics.h         # Font measurement system
│   │   ├── FontMetrics.cpp
│   │   ├── TextLayout.h          # Text line breaking & alignment
│   │   └── TextLayout.cpp
│   │
│   └── main.cpp                  # CLI entry point
│
├── tests/
│   ├── test_edge_values.cpp
│   ├── test_layout_node.cpp
│   ├── test_block_layout.cpp
│   ├── test_flex_layout.cpp
│   ├── test_parser.cpp
│   └── test_text_layout.cpp
│
├── visualizer/
│   └── text_layout.html          # Browser-based text layout visualizer
│
└── examples/
    ├── simple_block.txt          # Test inputs
    ├── simple_flex.txt
    └── nested_layout.txt
```

## Comparison to Real WebKit

| This Project | Real WebKit |
|--------------|-------------|
| Simplified input format | Full HTML/CSS parser |
| Two layout modes | Dozens of layout modes |
| No paint/composite | Full rendering pipeline |
| Single-threaded | Multi-threaded architecture |
| Basic optimization | Highly optimized for performance |
| ~1000 lines of code | Millions of lines |

This comparison demonstrates understanding rather than claiming equivalence.

## Learning Outcomes

Completing this project provides understanding of:

1. How browsers convert CSS properties into visual layout
2. How layout trees represent document structure
3. How box model calculations work
4. How flexbox algorithm determines sizes and positions
5. How text layout and line breaking works in browsers
6. How to write clean, testable C++ for complex algorithms
7. How to balance correctness with performance
8. How to follow specifications to implement standards

## Architecture Mapping to WebKit

| This Component | WebKit Equivalent |
|----------------|-------------------|
| LayoutNode | RenderObject / LayoutBox |
| LayoutTree | RenderTree |
| BlockLayout | BlockFormattingContext |
| FlexLayout | FlexFormattingContext |
| TextLayout | InlineFormattingContext / LineBreaker |
| FontMetrics | FontCascade / SimpleFontData |
| OutputFormatter | RenderTreeAsText (WebKit's layout dump tool) |
| LayoutEngine | FrameView / LayoutContext |

The concepts are the same; the scope is reduced.

## Documentation

All detailed documentation is in the `docs/` folder (gitignored for privacy):

## Build Instructions

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
make

# Run tests
./layout_tests

# Run engine
./layout_engine ../examples/simple_block.txt
```

## Usage

### Layout Mode

```bash
# Run with an input file (uses default 1024x768 viewport)
./layout_engine examples/simple_block.txt

# Specify custom viewport dimensions
./layout_engine examples/simple_flex.txt --viewport 800 600

# Output in JSON format
./layout_engine examples/nested_layout.txt --json

# Combine options
./layout_engine examples/complex_example.txt --viewport 1280 800 --json
```

### Text Layout Mode

```bash
# Basic text layout (outputs JSON)
./layout_engine --text-layout "The quick brown fox jumps over the lazy dog"

# Set container width and alignment
./layout_engine --text-layout "Hello world" --width 200 --align center

# Customize font metrics
./layout_engine --text-layout "Some text" --width 300 --char-width 10 --line-height 24

# All text layout options
./layout_engine --text-layout "text" --width 200 --align justify --char-width 8 --line-height 20 --letter-spacing 1
```

**Text layout options:**
- `--width <px>`: Container width (default: 200)
- `--align <mode>`: left | center | right | justify (default: left)
- `--char-width <px>`: Character width (default: 8)
- `--line-height <px>`: Line height (default: 20)
- `--letter-spacing <px>`: Letter spacing (default: 0)

### Text Layout Visualizer

A browser-based visualizer compares your C++ engine output side-by-side with native browser rendering.

```bash
# 1. Generate JSON from the engine
cd build
./layout_engine --text-layout "Your text here" --width 200 --align justify > ../visualizer/layout_data.json

# 2. Serve the visualizer
cd ../visualizer
python3 -m http.server 8080

# 3. Open http://localhost:8080/text_layout.html
```

### Input File Format

The engine uses a simplified CSS-like syntax:

```
container {
    display: flex;
    flex-direction: row;
    justify-content: space-between;
    width: 800;
    height: 600;

    child {
        display: block;
        width: 200;
        height: 100;
        margin: 10;
        padding: 5;
    }
}
```

**Supported Properties:**
- `display`: block | flex | none
- `width`, `height`: numeric values
- `margin`, `padding`, `border`: 1, 2, or 4 values (CSS shorthand)
- `flex-direction`: row | column
- `justify-content`: flex-start | center | flex-end | space-between
- `align-items`: flex-start | center | flex-end | stretch

### Example Output

**Console Output:**
```
=== Layout Results ===
Viewport: 1024x768

page: x=0.0, y=0.0, w=1024.0, h=768.0
  header: x=0.0, y=0.0, w=1024.0, h=80.0
  content: x=0.0, y=80.0, w=1024.0, h=638.0
    sidebar: x=0.0, y=0.0, w=250.0, h=638.0
    main: x=250.0, y=0.0, w=774.0, h=638.0
  footer: x=0.0, y=718.0, w=1024.0, h=50.0
```

**JSON Output:**
```json
{
  "id": "page",
  "x": 0.0,
  "y": 0.0,
  "width": 1024.0,
  "height": 768.0,
  "children": [...]
}
```

**Text Layout JSON Output:**
```json
{
  "text": "The quick brown fox jumps over the lazy dog.",
  "align": "left",
  "container_width": 200.0,
  "char_width": 8.0,
  "line_height": 20.0,
  "letter_spacing": 0.0,
  "total_width": 200.0,
  "total_height": 60.0,
  "lines": [
    {"words": ["The", "quick", "brown", "fox", "jumps"], "width": 200.0, "x_offset": 0.0, "y_position": 0.0, "word_spacing": 8.0, "is_justified": false},
    {"words": ["over", "the", "lazy", "dog."], "width": 152.0, "x_offset": 0.0, "y_position": 20.0, "word_spacing": 8.0, "is_justified": false}
  ]
}
```