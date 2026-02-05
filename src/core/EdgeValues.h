#ifndef EDGE_VALUES_H
#define EDGE_VALUES_H

class EdgeValues{
    public: 
    //initalizes all edges to zero -> element with no margin/padding/border
    EdgeValues();

    explicit EdgeValues(float all);

    EdgeValues(float top, float right, float bottom, float left);

    //getters
    float top() const {return m_top;}
    float right() const {return m_right;}
    float bottom() const {return m_bottom;}
    float left() const {return m_left;}

    //calculations
    float horizontal() const {return m_left + m_right;}
    float vertical() const {return m_top + m_bottom;}

    private:
    //member variables
    float m_top;
    float m_right;
    float m_bottom;
    float m_left;
};

#endif