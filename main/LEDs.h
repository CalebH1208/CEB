#ifndef LED_H
#define LED_H
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"


 void LED_on(int gpio);
 void LED_off(int gpio);
 void configure_led(int gpio);

#endif // LED_H