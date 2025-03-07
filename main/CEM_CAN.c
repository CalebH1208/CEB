#include "CEM_CAN.h"


static const char *TAG = "CEM_CAN";

esp_err_t CAN_init(gpio_num_t CAN_tx, gpio_num_t CAN_rx, valid_CAN_speeds_t bus_speed){
    twai_general_config_t global_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN_tx, CAN_rx, TWAI_MODE_NORMAL);
    twai_timing_config_t timing_config;
    switch(bus_speed){
        case(CAN_1MBITS):
            twai_timing_config_t timing_1000000 = TWAI_TIMING_CONFIG_1MBITS();
            timing_config = timing_1000000;
            break;
        case(CAN_500KBITS):
            twai_timing_config_t timing_500000 = TWAI_TIMING_CONFIG_500KBITS();
            timing_config = timing_500000;
            break;
        case(CAN_250KBITS):
            twai_timing_config_t timing_250000 = TWAI_TIMING_CONFIG_250KBITS();
            timing_config = timing_250000;
            break;
        case(CAN_100KBITS):
            twai_timing_config_t timing_100000 = TWAI_TIMING_CONFIG_100KBITS();
            timing_config = timing_100000;
            break;
        case(CAN_50KBITS):
            twai_timing_config_t timing_50000 = TWAI_TIMING_CONFIG_50KBITS();
            timing_config = timing_50000;
            break;
        default:
            return ESP_ERR_INVALID_ARG;
   }
   twai_filter_config_t filter_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

   ESP_ERROR_CHECK(twai_driver_install(&global_config, &timing_config, &filter_config));
   ESP_ERROR_CHECK(twai_start());
   ESP_LOGI(TAG,"CAN init success :)");
   return ESP_OK;
}


esp_err_t CAN_transmit_message(uint32_t ID, uint64_t data){
    uint8_t* piecewise_data = (uint8_t*)&data;

    twai_message_t CAN_frame;
    CAN_frame.identifier = ID;
    CAN_frame.data_length_code = 8;
    for(int i =0; i < 8; i++){
        CAN_frame.data[i] = piecewise_data[i];
    }

    return twai_transmit(&CAN_frame, pdMS_TO_TICKS(20));
}