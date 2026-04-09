#ifndef TEXT_LAYOUT_H
#define TEXT_LAYOUT_H

#include <string>
#include <vector>
#include "text/FontMetrics.h"

enum class TextAlign
{
    Left,
    Center,
    Right,
    Justify
};

struct TextLine
{
    std::vector<std::string> words;
    float width;
    float x_offset;
    float y_position;
    float word_spacing;
    bool is_justified;

    TextLine() : width(0.0f), x_offset(0.0f), y_position(0.0f), word_spacing(0.0f), is_justified(false) {}
};

struct TextLayoutResult
{
    std::vector<TextLine> lines;
    float total_width;
    float total_height;
    TextLayoutResult() : total_width(0.0f), total_height(0.0f) {}
};

class TextLayout
{
public:
    TextLayout();

    explicit TextLayout(const FontMetrics &metrics);

    // main layout function
    TextLayoutResult layout(const std::string &text,
                            float container_width,
                            TextAlign align = TextAlign::Left) const;

    const FontMetrics &getFontMetrics() const { return m_font_metrics; }
    void setFontMetrics(const FontMetrics &metrics) { m_font_metrics = metrics; }

private:
    FontMetrics m_font_metrics;

    // helper: split text into words (by whitespace)
    std::vector<std::string> splitIntoWords(const std::string &text) const;

    // helper: calculate horizontal offset for a line based on alignment
    float calculateLineXOffset(float line_width, float container_width, TextAlign align) const;

    // helper: measure width of a line with spaces between words
    float measureLineWidth(const std::vector<std::string> &words) const;

    float calculateJustifiedSpacing(const std::vector<std::string> &words,
                                    float container_width) const;
};

#endif