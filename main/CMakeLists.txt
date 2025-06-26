idf_component_register(SRCS "main.cpp"
                            "Clock.cpp"
                    PRIV_REQUIRES spi_flash
                    INCLUDE_DIRS "include")
