#include "CEM_ADC.h"

esp_err_t ADC_init(ADC_handle_t * handle){
    return adc_oneshot_config_channel( handle->unit_handle,  handle->channel,  handle->chan_config);
}

esp_err_t ADC_get_reading(ADC_handle_t * handle, int * data){
    int raw_data = 0;
    esp_err_t err = adc_oneshot_read(handle->unit_handle, handle->channel,&raw_data);
    if(err == ESP_OK)return err;
    err = adc_cali_raw_to_voltage(handle->calibrater,raw_data,data);
    return err;
}