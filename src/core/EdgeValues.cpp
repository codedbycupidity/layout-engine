#include "core/EdgeValues.h"

//constructor implementations

//default constructor
EdgeValues::EdgeValues()
    : m_top(0.0f)      // Initialize top to 0.0
    , m_right(0.0f)    // Initialize right to 0.0
    , m_bottom(0.0f)   // Initialize bottom to 0.0
    , m_left(0.0f)     // Initialize left to 0.0
{
    //all initalization done above
}

//set all edges to same value
EdgeValues::EdgeValues(float all)
    : m_top(all)       // Set top to the parameter value
    , m_right(all)     // Set right to the parameter value
    , m_bottom(all)    // Set bottom to the parameter value
    , m_left(all)      // Set left to the parameter value
{
    //all initalization done above
}

//set edges independently
EdgeValues::EdgeValues(float top, float right, float bottom, float left)
    : m_top(top)       // Initialize m_top with parameter 'top'
    , m_right(right)   // Initialize m_right with parameter 'right'
    , m_bottom(bottom) // Initialize m_bottom with parameter 'bottom'
    , m_left(left)     // Initialize m_left with parameter 'left'
{
   //all initaliztion done above
}