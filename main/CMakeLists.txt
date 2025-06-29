idf_component_register(SRCS "main.cpp"
                            "Clock.cpp"
                            "Wifi.cpp"
                            "Display.cpp"
                    PRIV_REQUIRES spi_flash
                                  esp_wifi
                                  nvs_flash
                    INCLUDE_DIRS "include")
