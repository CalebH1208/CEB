idf_component_register(SRCS "CEM_EGT.c" "Spi.c" "main.c" "LEDs.c"
                    PRIV_REQUIRES spi_flash esp_driver_gpio esp_driver_spi driver
                    INCLUDE_DIRS "")
