idf_component_register(SRCS "CEM_EGT.c" "CEM_CAN.c" "Spi.c" "main.c" "LEDs.c" "CEM_ADC.c" "CEM.c"
                    PRIV_REQUIRES spi_flash esp_driver_gpio esp_driver_spi driver esp_adc
                    INCLUDE_DIRS "")
