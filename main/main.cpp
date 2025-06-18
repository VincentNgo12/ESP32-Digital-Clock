/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

// ESP-IDF requires app_main to have C linkage
extern "C" void app_main();

// Classes declarations
class SystemInfo;
class Rebooter;

void app_main() {
    printf("Hello world from C++!\n");

    SystemInfo::print_chip_info();
    Rebooter::countdown_and_restart(10);
}

// SystemInfo class
class SystemInfo
{
public:
    static void print_chip_info()
    {
        esp_chip_info_t chip_info;
        esp_chip_info(&chip_info);

        printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
               CONFIG_IDF_TARGET,
               chip_info.cores,
               (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
               (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
               (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
               (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

        unsigned major_rev = chip_info.revision / 100;
        unsigned minor_rev = chip_info.revision % 100;
        printf("silicon revision v%d.%d, ", major_rev, minor_rev);

        uint32_t flash_size;
        if (esp_flash_get_size(nullptr, &flash_size) != ESP_OK)
        {
            printf("Failed to get flash size\n");
            return;
        }

        printf("%" PRIu32 "MB %s flash\n", flash_size / (1024 * 1024),
               (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

        printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
    }
};

// Rebooter class
class Rebooter
{
public:
    static void countdown_and_restart(int seconds)
    {
        for (int i = seconds; i >= 0; --i)
        {
            printf("Restarting in %d seconds...\n", i);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }

        printf("Restarting now.\n");
        fflush(stdout);
        esp_restart();
    }
};