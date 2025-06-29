#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "driver/spi_master.h"

class Display {
public:
    void init();                       // Initialize display
    void draw_pixel(uint16_t x, uint16_t y, uint16_t color);  // Optional: draw 1 pixel
    void fill_screen(uint16_t color);  // Optional: fill entire screen
    void draw_bitmap(const void* data); // Optional: draw raw framebuffer
private:
    esp_lcd_panel_handle_t panel = nullptr;
};

#endif // DISPLAY_HPP