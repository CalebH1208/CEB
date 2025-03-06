
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "LEDs.h"


static const char *TAG = "Lora Test";

void app_main(void)
{
    int LED1 = 5;

    configure_led(LED1);

    while (1) {
        LED_on(LED1);
        vTaskDelay(pdMS_TO_TICKS(200));
        LED_off(LED1);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
