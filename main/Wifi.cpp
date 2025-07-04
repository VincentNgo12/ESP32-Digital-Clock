#include <cstring>
#include "Wifi.hpp"
#include "wifi_secrets.hpp"

#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

// Event group and bit for Wi-Fi connection
static EventGroupHandle_t wifi_event_group; // Event group bits
const int WIFI_CONNECTED_BIT = BIT0;        // Wi-fi connected event (bit 0)

static const char* TAG = "WiFi";            // Tag for ESP_LOGI()

/* ===================================================================
   This function handles system events ("Wi-Fi connected" or "Got IP")
   ===================================================================*/
static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data) {
    // Wi-fi STA Mode
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    // Wi-fi disconnected
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGW(TAG, "Disconnected, retrying...");
        esp_wifi_connect();
    // When IP is obtained
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);  // Set event group bits (Wi-fi connected)
    }
}

/*=======================================
  This function setups wi-fi station mode
  =======================================*/
void wifi_init_sta() {
    // Initialize NVS (Non-Volatile Storage) to store wi-fi configuration
    ESP_LOGI(TAG, "Initializing NVS");
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // *** Start setting up network interface
    ESP_LOGI(TAG, "Initializing Wi-Fi");
    wifi_event_group = xEventGroupCreate(); // Create event group to track connection status

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    // Use default Wi-fi init config
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Event handlers
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    // Register wi-fi event handlers
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    // Set SSID and password
    wifi_config_t wifi_config = {};
    strncpy((char*)wifi_config.sta.ssid, WIFI_SSID, sizeof(wifi_config.sta.ssid));
    strncpy((char*)wifi_config.sta.password, WIFI_PASS, sizeof(wifi_config.sta.password));
    // Set minimum auth level to WPA2
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));                // Set mode to Station (not SoftAP).
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));  // Apply your Wi-Fi config
    ESP_ERROR_CHECK(esp_wifi_start());                                // Start the Wi-Fi driver!

    ESP_LOGI(TAG, "Connecting to Wi-Fi...");

    // Wait until connected
    EventBits_t bits = xEventGroupWaitBits(wifi_event_group,
                                           WIFI_CONNECTED_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           pdMS_TO_TICKS(10000));  // timeout in 10 seconds

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "Connected to Wi-Fi SSID: %s", WIFI_SSID);
    } else {
        ESP_LOGW(TAG, "Failed to connect to Wi-Fi");
    }
}
