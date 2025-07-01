#include "Display.hpp"
#include "esp_log.h"
#include "driver/gpio.h"

#define TAG "Display"

// Define your pin numbers here
#define LCD_HOST       SPI2_HOST
#define PIN_NUM_MOSI   23 // MOSI Pin
#define PIN_NUM_CLK    18 // CLK Pin
#define PIN_NUM_CS     5  // Chip Select Pin
#define PIN_NUM_DC     21 // Register/Data Selection Pin
#define PIN_NUM_RST    22 // Reset Pin
#define PIN_NUM_BK     4  // Backlight control

#define LCD_WIDTH  480
#define LCD_HEIGHT 320

/*======================================================================

  ======================================================================*/

void Display::init() {
    // Init backlight
    gpio_config_t bk_cfg = {
        // Configures your backlight pin (PIN_NUM_BK) as a regular output pin. 
        .pin_bit_mask = 1ULL << PIN_NUM_BK,     // Select Backlight pin (4)
        .mode = GPIO_MODE_OUTPUT,   
        .pull_up_en = GPIO_PULLUP_DISABLE,      // No pull-up
        .pull_down_en = GPIO_PULLDOWN_DISABLE,  // No pull-down
        .intr_type = GPIO_INTR_DISABLE          // No interrupts
    };
    gpio_config(&bk_cfg);
    gpio_set_level((gpio_num_t)PIN_NUM_BK, 1);  // Turn on backlight (always HIGH)

    // Init SPI bus
    spi_bus_config_t buscfg = {
        .mosi_io_num = PIN_NUM_MOSI,    // Select MOSI pin
        .miso_io_num = -1,              // *** MISO not needed (only send data to display)
        .sclk_io_num = PIN_NUM_CLK,     // Select CLK pin
        .quadwp_io_num = -1,            // Unused
        .quadhd_io_num = -1,            // Unuse
        // *** Largest data transfer size: entire screen in bytes for 2 bytes per pixels
        .max_transfer_sz = LCD_WIDTH * LCD_HEIGHT * 2 + 8
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO)); /* SPI_DMA_CH_AUTO: let ESP-IDF pick an 
                                                                                available DMA channel for faster SPI.*/

    // SPI panel IO config
    esp_lcd_panel_io_handle_t io_handle;
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = PIN_NUM_DC,      // Select DC pin (Distinguishes between data and command)
        .cs_gpio_num = PIN_NUM_CS,      // Select CS pin (LOW = active)
        .pclk_hz = 20 * 1000 * 1000,    // SPI clock speed (20 MHz — safe for ILI9488 Driver)
        .lcd_cmd_bits = 8,              // 8 bits per command
        .lcd_param_bits = 8,            // 8 bits per data
        .spi_mode = 0,
        .trans_queue_depth = 10,        // How many SPI transfers can be queued at once
        .on_color_trans_done = nullptr,
        .user_ctx = nullptr,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));

    // ILI9488 panel config
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = PIN_NUM_RST,          // Select reset pin
        .color_space = ESP_LCD_COLOR_SPACE_RGB, // RGB color space
        .bits_per_pixel = 16,  // RGB565
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_ili9488(io_handle, &panel_config, &panel)); // Using ILI9488 Display Driver

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel));                // Toggle reset pin
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel));                 // Send ILI9488 initialization command sequence (done by driver)
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel, true));   // Invert color
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel, false, false)); // Flip screen if rotated
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel, false));       // Swap X and Y axis (unsued)
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel, true));    // Turn on display
}


void Display::draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    esp_lcd_panel_draw_bitmap(panel, x, y, x + 1, y + 1, &color);
}


void Display::fill_screen(uint16_t color) {
    uint16_t* buffer = new uint16_t[LCD_WIDTH * LCD_HEIGHT];
    for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; ++i)
        buffer[i] = color;
    esp_lcd_panel_draw_bitmap(panel, 0, 0, LCD_WIDTH, LCD_HEIGHT, buffer);
    delete[] buffer;
}
