#include "Clock.hpp"

#include <ctime>
#include <string>
#include <cstdio>

#include "esp_log.h"
#include "esp_sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "Clock"

// Edmonton Time Zone
#define TZ "MST7MDT,M3.2.0/2,M11.1.0/2"

void Clock::init() {
    setenv("TZ", TZ, 1);  // Set timezone
    tzset();              // Apply timezone

    ESP_LOGI(TAG, "Initializing SNTP...");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);   // Set SNTP poll mode
    sntp_setservername(0, "pool.ntp.org");     // Set source NTP server
    sntp_init();                               // Start SNTP client (beign syncing in background)

    // Wait for system time to be set
    time_t now = 0;
    struct tm timeinfo = {};     // Struct to store (hours, min, sec, etc.)
    int retry = 0;
    const int retry_count = 10;  // Max number of syncing retries

    // Keep syncing while the time is invalid
    while (timeinfo.tm_year < (2020 - 1900) && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for SNTP sync... (%d/%d)", retry, retry_count);
        vTaskDelay(pdMS_TO_TICKS(2000)); 
        time(&now);                         // Gets current time
        localtime_r(&now, &timeinfo);       // Converts it to local time format (tm_hour, etc.)
    }

    // Final check (if sync fails log a warning)
    if (timeinfo.tm_year < (2020 - 1900)) {
        ESP_LOGW(TAG, "Time sync failed, RTC may be inaccurate.");
    } else {
        ESP_LOGI(TAG, "Time synchronized: %02d:%02d:%02d",
                 timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec); // Time synchronization successful
    }
}

std::string Clock::get_time_str() {
    time_t now;
    struct tm timeinfo;
    time(&now);                     // Get current time
    localtime_r(&now, &timeinfo);   // Convert to local format

    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d",
             timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec); // Formatted time string
    return std::string(buf);
}
