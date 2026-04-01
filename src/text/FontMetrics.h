#ifndef FONT_METRICS_H
#define FONT_METRICS_H

#include <string>

class FontMetrics
{
public:
    // constructor with default values
    FontMetrics(float char_width = 8.0f,
                float line_height = 20.0f,
                float letter_spacing = 0.0f);

    float getWordWidth(const std::string &word) const;
    float getCharWidth() const { return m_char_width; }
    float getLineHeight() const { return m_line_height; }
    float getLetterSpacing() const { return m_letter_spacing; }

    void setCharWidth(float width) { m_char_width = width; }
    void setLineHeight(float height) { m_line_height = height; }
    void setLetterSpacing(float spacing) { m_letter_spacing = spacing; }

private:
    float m_char_width;
    float m_line_height;
    float m_letter_spacing;
};

#endif