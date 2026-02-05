# Mini Layout Engine

A C++ implementation of core layout algorithms demonstrating understanding of browser rendering fundamentals. Built as preparation for the WebKit Layout and Rendering team internship.

## Project Overview

This project implements a simplified layout engine in C++ that:
- Parses a simplified layout description format (CSS-like properties)
- Constructs a layout tree
- Implements fundamental layout algorithms (Block layout + Flexbox basics)
- Calculates positions and dimensions
- Includes comprehensive testing

This is NOT a toy project—it implements real layout algorithms following web standards, just with reduced scope.

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

### 4. Layout Engine
- Coordinates layout passes
- Handles dependencies
- Computes final positions

### 5. Testing Framework
- Unit tests for layout calculations
- Regression test suite
- Performance benchmarks

### Optional: Visualizer (HTML/JS)
- Takes JSON output from C++ engine
- Renders boxes visually
- Shows computed dimensions/positions

**Note**: The visualizer is optional. The C++ engine is the focus.

## Project Scope

### Included Features 

- **Box model**: margin, border, padding, content
- **Block layout**: Vertical stacking, width/height calculation
- **Basic flexbox**: flex-direction (row/column), justify-content, align-items
- **Positioning**: Normal flow only (no absolute/fixed)
- **Properties**: Width, height, margin, padding, display, flex properties
- **Testing**: Comprehensive unit and integration tests

### Excluded Features 

These are too complex for a week-long implementation but should be understood conceptually:

- Float layout
- CSS Grid layout
- Absolute/fixed positioning
- Inline layout / text rendering
- CSS cascade and specificity
- Full CSS parser
- Paint / composite stages
- JavaScript integration
- Tables, form elements
- Complex flexbox features (wrapping, flex-basis, shrink/grow ratios)

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
├── docs/                         # Documentation (gitignored)
│   ├── ARCHITECTURE.md           # System design
│   ├── IMPLEMENTATION_GUIDE.md   # Implementation guidance
│   ├── LEARNING_RESOURCES.md     # C++ and layout learning path
│   ├── INTERVIEW_PREP.md         # Interview preparation
│   └── LANGUAGE_GUIDELINES.md    # Terminology guidelines
│
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
│   └── main.cpp                  # CLI entry point
│
├── tests/
│   ├── test_edge_values.cpp
│   ├── test_layout_node.cpp
│   ├── test_block_layout.cpp
│   ├── test_flex_layout.cpp
│   └── test_parser.cpp
│
└── examples/
    ├── simple_block.txt          # Test inputs
    ├── simple_flex.txt
    └── nested_layout.txt
```

## Implementation Timeline

### Days 1-2: C++ Fundamentals + Design
- Learn/review C++ basics (classes, pointers, STL)
- Understand box model and block layout
- Design class structure
- Set up project with CMake

### Days 3-4: Core + Block Layout
- Implement LayoutNode, EdgeValues, tree structure
- Implement block layout algorithm
- Write tests for block layout
- Achieve basic layout functionality

### Days 5-6: Flexbox Implementation
- Study flexbox specification
- Implement simplified flexbox algorithm
- Write tests for flexbox
- Verify correctness

### Day 7: Polish + Interview Prep
- Add more test cases
- Complete documentation
- Create example outputs
- Prepare demo and talking points
- (Optional) Build visualizer

## Success Criteria

Interview readiness is achieved when the following can be demonstrated:

- **Working code**: Input → layout calculation → output
- **Algorithm explanation**: Clear explanation of block layout and flexbox implementations
- **Clean C++ code**: Smart pointers, RAII, const-correctness demonstrated
- **Design rationale**: Clear reasoning for class structure decisions
- **Test coverage**: Comprehensive unit and integration tests
- **Trade-off awareness**: Understanding of simplifications made
- **WebKit connection**: Ability to relate implementation to real browser layout engines

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
5. How to write clean, testable C++ for complex algorithms
6. How to balance correctness with performance
7. How to follow specifications to implement standards

## Architecture Mapping to WebKit

| This Component | WebKit Equivalent |
|----------------|-------------------|
| LayoutNode | RenderObject / LayoutBox |
| LayoutTree | RenderTree |
| BlockLayout | BlockFormattingContext |
| FlexLayout | FlexFormattingContext |
| LayoutEngine | FrameView / LayoutContext |

The concepts are the same; the scope is reduced.

## Documentation

All detailed documentation is in the `docs/` folder (gitignored for privacy):

- **LEARNING_RESOURCES.md** - C++ learning path for Days 1-2
- **ARCHITECTURE.md** - System design and class structure
- **IMPLEMENTATION_GUIDE.md** - Step-by-step implementation guidance
- **INTERVIEW_PREP.md** - How to discuss this project
- **LANGUAGE_GUIDELINES.md** - Accurate terminology to use

## Interview Positioning

**Avoid**: "I built a browser layout engine"

**Use**: "I implemented core layout algorithms in C++ following the Flexbox specification. It's a simplified version of what WebKit does—it handles basic block layout and flexbox for calculating positions and dimensions. Building this helped me understand how layout engines work, and I'm excited to learn how WebKit handles the full complexity at scale."

## Project Value Proposition

1. **Directly relevant**: Implements the same types of algorithms WebKit engineers work on
2. **Achievable scope**: Realistic for one week of focused work
3. **Demonstrates learning**: Shows ability to pick up C++ and complex concepts quickly
4. **Conversation starter**: Provides concrete examples for technical discussions
5. **Honest approach**: Acknowledges simplifications while showing understanding

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

## Getting Started

1. Review `docs/LEARNING_RESOURCES.md` for C++ fundamentals
2. Study `docs/ARCHITECTURE.md` for system design
3. Follow `docs/IMPLEMENTATION_GUIDE.md` for step-by-step implementation
4. Refer to `docs/INTERVIEW_PREP.md` when preparing to discuss the project

## Goal

Build a working C++ layout engine that can be demonstrated and discussed confidently in a technical interview for the WebKit Layout and Rendering team.
