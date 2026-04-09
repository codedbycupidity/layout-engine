#include "text/TextLayout.h"
#include <sstream>
#include <algorithm>

TextLayout::TextLayout() : m_font_metrics(FontMetrics()) {}

TextLayout::TextLayout(const FontMetrics &metrics) : m_font_metrics(metrics) {}

std::vector<std::string> TextLayout::splitIntoWords(const std::string &text) const
{
    std::vector<std::string> words;
    std::istringstream stream(text);
    std::string word;

    while (stream >> word)
    {
        words.push_back(word);
    }
    return words;
}

float TextLayout::measureLineWidth(const std::vector<std::string> &words) const
{
    if (words.empty())
    {
        return 0.0f;
    }

    float total_width = 0.0f;
    float space_width = m_font_metrics.getCharWidth(); // space is one character wide

    for (size_t i = 0; i < words.size(); i++)
    {
        total_width += m_font_metrics.getWordWidth(words[i]);

        // add space width between words but not after the last word
        if (i < words.size() - 1)
        {
            total_width += space_width;
        }
    }
    return total_width;
}

float TextLayout::calculateLineXOffset(float line_width, float container_width, TextAlign align) const
{

    switch (align)
    {
    case TextAlign::Left:
        return 0.0f;
    case TextAlign::Right:
        return container_width - line_width;
    case TextAlign::Center:
        return (container_width - line_width) / 2.0f;
    case TextAlign::Justify:
        // justified text starts at 0 (left edge)
        return 0.0f;
    default:
        return 0.0f;
    }
}

float TextLayout::calculateJustifiedSpacing(const std::vector<std::string> &words, float container_width) const
{
    if (words.size() <= 1)
    {
        // cant justify a single word
        return m_font_metrics.getCharWidth();
    }

    float words_width = 0.0f;
    for (const auto &word : words)
    {
        words_width += m_font_metrics.getWordWidth(word);
    }

    // calculate how much space we need to distribute between words
    float avaliable_space = container_width - words_width;

    // number of gaps between words
    size_t num_gaps = words.size() - 1;

    // divide space evenly among gaps
    return avaliable_space / num_gaps;
}

TextLayoutResult TextLayout::layout(const std::string &text,
                                    float container_width, TextAlign align) const
{
    TextLayoutResult result;

    if (text.empty())
    {
        return result;
    }

    std::vector<std::string> words = splitIntoWords(text);
    if (words.empty())
    {
        return result;
    }

    // line breaking algorithm
    std::vector<std::string> current_line;
    float space_width = m_font_metrics.getCharWidth();

    for (const auto &word : words)
    {
        float word_width = m_font_metrics.getWordWidth(word);

        float potential_width;
        if (!current_line.empty())
        {
            potential_width = measureLineWidth(current_line) + space_width + word_width;
        }
        else
        {
            potential_width = word_width;
        }

        // word fits into line
        if (potential_width <= container_width)
        {
            current_line.push_back(word);
        }
        else
        {
            if (!current_line.empty())
            {
                TextLine line;
                line.words = current_line;
                line.width = measureLineWidth(current_line);
                result.lines.push_back(line);

                current_line.clear();
            }

            current_line.push_back(word);
        }
    }
    // adds last line
    if (!current_line.empty())
    {
        TextLine line;
        line.words = current_line;
        line.width = measureLineWidth(current_line);
        result.lines.push_back(line);
    }

    float current_y = 0.0f;
    float line_height = m_font_metrics.getLineHeight();

    for (size_t i = 0; i < result.lines.size(); i++)
    {
        auto &line = result.lines[i];
        bool isLastLine = (i == result.lines.size() - 1);
        bool shouldJustify = (align == TextAlign::Justify) && !isLastLine;

        if (shouldJustify)
        {
            line.word_spacing = calculateJustifiedSpacing(line.words, container_width);
            line.is_justified = true;
            line.x_offset = calculateLineXOffset(line.width, container_width, align);
        } else {
            line.word_spacing = m_font_metrics.getCharWidth();
            line.is_justified = false;
            line.x_offset = calculateLineXOffset(line.width, container_width, align);
        }

        line.y_position = current_y;
        current_y += line_height;

        result.total_width = std::max(result.total_width, line.width);
    }

    result.total_height = current_y;

    return result;
}