#include "TextRenderer.hpp"
#include "font8x8_basic.h"
#include "Display.hpp"

/*==============================================================
  This class along with its methods allow user to draw texts
  from provided bitmaps ("font8x8_basic.h") to the display
  ==============================================================*/

// Draw a single character on display (with scaling factor)
void TextRenderer::draw_char(uint16_t x, uint16_t y, char c, uint16_t color, uint8_t scale) {
    if (c < 0 || c > 127) return;  // Only ASCII supported

    const uint8_t* glyph = font8x8_basic[(uint8_t)c];   // Gets bitmaps of given char (an array of bytes)

    for (int row = 0; row < 8; ++row) {
        uint8_t bits = glyph[row];  // bits of current row
        for (int col = 0; col < 8; ++col) {
            if (bits & (1 << col)) {
                // Draw scaled pixel block
                for (int dy = 0; dy < scale; ++dy) {
                    for (int dx = 0; dx < scale; ++dx) {
                        Display::draw_pixel(x + col * scale + dx, y + row * scale + dy, color);
                    }
                }
            }
        }
    }
}


// Method to draw string on screen
void TextRenderer::draw_string(uint16_t x, uint16_t y, const std::string& text, uint16_t color, uint8_t scale) {
    for (size_t i = 0; i < text.length(); ++i) {
        draw_char(x + i * 8 * scale, y, text[i], color, scale);
    }
}


// Method to draw string in the center of screen
void TextRenderer::draw_centered_string(uint16_t center_x, uint16_t center_y, const std::string& text, uint16_t color, uint8_t scale) {
    uint16_t total_width = text.length() * 8 * scale;   // Total width of string 
    uint16_t total_height = 8 * scale;                  // Height of string
    uint16_t start_x = center_x - total_width / 2;      // Calculate start X pos
    uint16_t start_y = center_y - total_height / 2;     // Calculate start Y pos
    draw_string(start_x, start_y, text, color, scale);  // Use existing method
}