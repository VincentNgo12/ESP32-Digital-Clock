#ifndef TEXTRENDERER_HPP
#define TEXTRENDERER_HPP

#include <string>
#include <cstdint>

class TextRenderer {
public:
    // Draw a single character at (x, y) with specified color and scale
    static void draw_char(uint16_t x, uint16_t y, char c, uint16_t color, uint8_t scale = 1);

    // Draw a string at (x, y) with specified color and scale
    static void draw_string(uint16_t x, uint16_t y, const std::string& text, uint16_t color, uint8_t scale = 1);

    // Draw a string centered at (center_x, center_y)
    static void draw_centered_string(uint16_t center_x, uint16_t center_y, const std::string& text, uint16_t color, uint8_t scale = 1);
};

#endif