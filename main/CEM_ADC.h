#ifndef CEM_ADC_H
#define CEM_ADC_H

#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_mac.h"

typedef struct{
    adc_oneshot_unit_handle_t unit_handle;
    adc_oneshot_chan_cfg_t * chan_config;
    adc_cali_handle_t calibrater;
    adc_channel_t channel;
} ADC_handle_t;

esp_err_t ADC_init(ADC_handle_t* handle); 

esp_err_t ADC_get_reading(ADC_handle_t * handle, int * inputs);

#endif //CEM_ADC_H