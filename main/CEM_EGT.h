#ifndef CEM_EGT_H
#define CEM_EGT_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "sdkconfig.h"
#include "Spi.h"

#define SPI_BUS_SPEED 5000000

#define EGT_TEMP_MASK 0b0111111111111100

typedef struct {
    spi_device_handle_t* device;
    spi_host_device_t host;
    gpio_num_t miso;
    gpio_num_t clk;
    gpio_num_t nss;
}EGT_handle_t;

esp_err_t EGT_init(EGT_handle_t * handle);

esp_err_t EGT_sample(EGT_handle_t * handle, float* data);

#endif //CEM_EGT_H