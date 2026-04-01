#include "text/FontMetrics.h"

FontMetrics::FontMetrics(float char_width, float line_height, float letter_spacing)
    : m_char_width(char_width), m_line_height(line_height), m_letter_spacing(letter_spacing) {}

float FontMetrics ::
    getWordWidth(const std::string &word) const
{
    if (word.empty())
    {
        return 0.0f;
    }

    float base_width = word.length() * m_char_width;
    float spacing_width = (word.length() - 1) * m_letter_spacing;

    return base_width + spacing_width;
}