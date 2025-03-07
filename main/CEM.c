#include "CEM.h"

static const char *TAG = "CEM Ritual";

void main_ritual(void * params){
    main_ritual_params_t * MP = (main_ritual_params_t*) params;
    esp_err_t err;
    TickType_t curr_ticks;
    while(1){
        curr_ticks = xTaskGetTickCount();
        // EGTs are first and then the ADCs
        float EGT_data[MP->EGT_arr_len]; 
        for(int i = 0;i<MP->EGT_arr_len; i++){
            err = EGT_sample(MP->EGT_arr+i, EGT_data + i);
            if(err != ESP_OK) ESP_LOGE(TAG,"error getting EGT%d value : %d",i+1,err);
        }
        int ADC_data[MP->ADC_arr_len];
        for(int i =0; i<MP->ADC_arr_len;i++){
            err = ADC_get_reading(MP->ADC_arr + i, ADC_data + i);
            if(err != ESP_OK) ESP_LOGE(TAG,"error getting ADC%d value: %d",i+1,err);
        }
        for(int i =0; i<(MP->EGT_arr_len + 1)/2; i++){
            uint64_t CAN_EGT_data = 0;

            if(MP->EGT_arr_len <= 2 * i + 1){
                CAN_EGT_data = (uint64_t) *((uint32_t*)(EGT_data + i*2));
            } else{
                CAN_EGT_data = *((uint64_t*)(EGT_data + i*2));
            }

            err = CAN_transmit_message(CAN_ID_OFFSET+i,CAN_EGT_data);
            if(err != ESP_OK) ESP_LOGE(TAG,"error sending can ID:%d value: %d",CAN_ID_OFFSET + i,err);
            ESP_LOGI(TAG,"on CAN sending: %f | %f",EGT_data[i*2],EGT_data[i*2 +1]);
        }
        for(int i =0; i<MP->ADC_arr_len; i++){
            uint64_t CAN_ADC_data = ((uint64_t)(ADC_data[i])) << 32;
            i++;
            if(i < MP->ADC_arr_len)CAN_ADC_data |= ((uint64_t)(ADC_data[i]));

            err = CAN_transmit_message(CAN_ID_OFFSET+i,CAN_ADC_data);
            if(err != ESP_OK) ESP_LOGE(TAG,"error sending can ID:%d value: %d",CAN_ID_OFFSET + i,err);
            ESP_LOGI(TAG,"on CAN sending: %d | %d",ADC_data[i-1],ADC_data[i]);
        }
        xTaskDelayUntil(&curr_ticks,pdMS_TO_TICKS(1000/SAMPLE_N_SEND_HZ));
    }

}