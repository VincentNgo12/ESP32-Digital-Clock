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

#include "esp_flash.h"
#include "esp_system.h"

#include "Wifi.hpp"
#include "Clock.hpp"

// ESP-IDF requires app_main to have C linkage
extern "C" void app_main();


void app_main() {
    wifi_init_sta();  // Wi-fi initialization
    Clock::init();     // Setup RTC & sync SNTP
}
