
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "LEDs.h"

#include "CEM_ADC.h"
#include "CEM_CAN.h"
#include "CEM_EGT.h"
#include "CEM.h"


#define PRINT_RUN_STATS true // define this to enable debug printing

/*
TODOs:
Check on ADC_ATTEN  & ADC_BITWIDTH parameters 
check ADC pinout cause 3 on unit 1 and 5 on unit 2???
check on EGT setup just to make sure
check the adc calibrater stuff and see whats ups
flash a thing :)
*/


static const char *TAG = "CEM";

#define CAN_RX GPIO_NUM_48
#define CAN_TX GPIO_NUM_45

#define SPI_CLK GPIO_NUM_12
#define SPI_MISO GPIO_NUM_13

#define EGT1_NSS GPIO_NUM_5
#define EGT2_NSS GPIO_NUM_6
#define EGT3_NSS GPIO_NUM_7
#define EGT4_NSS GPIO_NUM_15

#define SOFTWARE_KILL GPIO_NUM_35

#define NUM_OF_ADCS 8
#define NUM_OF_EGTS 4

#define ADC1_CHANNEL ADC_CHANNEL_3
#define ADC1_UNIT ADC_UNIT_2

#define ADC2_CHANNEL ADC_CHANNEL_0
#define ADC2_UNIT ADC_UNIT_2

#define ADC3_CHANNEL ADC_CHANNEL_9
#define ADC3_UNIT ADC_UNIT_1

#define ADC4_CHANNEL ADC_CHANNEL_8
#define ADC4_UNIT ADC_UNIT_1

#define ADC5_CHANNEL ADC_CHANNEL_6
#define ADC5_UNIT ADC_UNIT_2

#define ADC6_CHANNEL ADC_CHANNEL_5
#define ADC6_UNIT ADC_UNIT_2

#define ADC7_CHANNEL ADC_CHANNEL_7
#define ADC7_UNIT ADC_UNIT_2

#define ADC8_CHANNEL ADC_CHANNEL_7
#define ADC8_UNIT ADC_UNIT_1

#define ADC_ATTEN  ADC_ATTEN_DB_12 // this will need to change most likely might need one for each unit based off sensors
#define ADC_BITWIDTH ADC_BITWIDTH_DEFAULT // also may need to change

void app_main(void)
{
    // calibrater might be able to just use one
    adc_cali_handle_t unit1_calibrater;

    const adc_cali_curve_fitting_config_t unit1_cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };

    adc_oneshot_unit_init_cfg_t unit1_config = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
        .clk_src = 0
    };

    adc_oneshot_unit_handle_t unit1_handle;
    esp_err_t err = adc_oneshot_new_unit(&unit1_config,&unit1_handle);
    if(err != ESP_OK){
        ESP_LOGE(TAG,"exiting unit1 handle creation error: %d",err);
        return;
    }

    adc_cali_handle_t unit2_calibrater;

    const adc_cali_curve_fitting_config_t unit2_cali_config = {
        .unit_id = ADC_UNIT_2,
        .atten = ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };

    adc_oneshot_unit_init_cfg_t unit2_config = {
        .unit_id = ADC_UNIT_2,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
        .clk_src = 0
    };

    adc_oneshot_unit_handle_t unit2_handle;
    err = adc_oneshot_new_unit(&unit2_config,&unit2_handle);
    if(err != ESP_OK){
        ESP_LOGE(TAG,"exiting unit2 handle creation error: %d",err);
        return;
    }

    err = adc_cali_create_scheme_curve_fitting(&unit1_cali_config ,&unit1_calibrater);
    err = adc_cali_create_scheme_curve_fitting(&unit2_cali_config ,&unit2_calibrater);
    if(err != ESP_OK){
        ESP_LOGE(TAG,"exiting calibration creation error: %d",err);
        return;
    }

    adc_oneshot_chan_cfg_t adc1_chan;
    ADC_handle_t adc1 = {
        .unit_handle = (ADC1_UNIT == ADC_UNIT_1)? unit1_handle: unit2_handle,
        .calibrater = (ADC1_UNIT == ADC_UNIT_1)? unit1_calibrater: unit2_calibrater,
        .channel = ADC1_CHANNEL,
        .chan_config = &adc1_chan,
    };
    adc_oneshot_chan_cfg_t adc2_chan;
    ADC_handle_t adc2 = {
        .unit_handle = (ADC2_UNIT == ADC_UNIT_1)? unit1_handle: unit2_handle,
        .calibrater = (ADC2_UNIT == ADC_UNIT_1)? unit1_calibrater: unit2_calibrater,
        .channel = ADC2_CHANNEL,
        .chan_config = &adc2_chan,
    };
    adc_oneshot_chan_cfg_t adc3_chan;
    ADC_handle_t adc3 = {
        .unit_handle = (ADC3_UNIT == ADC_UNIT_1)? unit1_handle: unit2_handle,
        .calibrater = (ADC3_UNIT == ADC_UNIT_1)? unit1_calibrater:unit2_calibrater,
        .channel = ADC3_CHANNEL,
        .chan_config = &adc3_chan,
    };
    adc_oneshot_chan_cfg_t adc4_chan;
    ADC_handle_t adc4 = {
        .unit_handle = (ADC4_UNIT == ADC_UNIT_1)? unit1_handle: unit2_handle,
        .calibrater = (ADC4_UNIT == ADC_UNIT_1)? unit1_calibrater: unit2_calibrater,
        .channel = ADC4_CHANNEL,
        .chan_config = &adc4_chan,
    };
    adc_oneshot_chan_cfg_t adc5_chan;
    ADC_handle_t adc5 = {
        .unit_handle = (ADC5_UNIT == ADC_UNIT_1)? unit1_handle: unit2_handle,
        .calibrater = (ADC5_UNIT == ADC_UNIT_1)? unit1_calibrater: unit2_calibrater,
        .channel = ADC5_CHANNEL,
        .chan_config = &adc5_chan,
    };
    adc_oneshot_chan_cfg_t adc6_chan;
    ADC_handle_t adc6 = {
        .unit_handle = (ADC6_UNIT == ADC_UNIT_1)? unit1_handle: unit2_handle,
        .calibrater = (ADC6_UNIT == ADC_UNIT_1)? unit1_calibrater: unit2_calibrater,
        .channel = ADC6_CHANNEL,
        .chan_config = &adc6_chan,
    };
    adc_oneshot_chan_cfg_t adc7_chan;
    ADC_handle_t adc7 = {
        .unit_handle = (ADC7_UNIT == ADC_UNIT_1)? unit1_handle: unit2_handle,
        .calibrater = (ADC7_UNIT == ADC_UNIT_1)? unit1_calibrater: unit2_calibrater,
        .channel = ADC7_CHANNEL,
        .chan_config = &adc7_chan,
    };
    adc_oneshot_chan_cfg_t adc8_chan;
    ADC_handle_t adc8 = {
        .unit_handle = (ADC8_UNIT == ADC_UNIT_1)? unit1_handle: unit2_handle,
        .calibrater = (ADC8_UNIT == ADC_UNIT_1)? unit1_calibrater: unit2_calibrater,
        .channel = ADC8_CHANNEL,
        .chan_config = &adc8_chan,
    };

    ADC_handle_t ADCs[NUM_OF_ADCS] = {adc1,adc2,adc3,adc4,adc5,adc6,adc7,adc8}; // might need to make these pointers maybe

    for(int i = 0;i<NUM_OF_ADCS; i++){
        err = ADC_init(ADCs + i);
        if(err != ESP_OK){
            ESP_LOGE(TAG,"exiting ADC%d initialization error: %d",i+1,err);
            return;
        }
    }

    spi_device_handle_t egt1_spi;
    EGT_handle_t EGT1 = {
        .device = &egt1_spi,
        .host = SPI3_HOST,
        .miso = SPI_CLK,
        .clk = SPI_CLK,
        .nss = EGT1_NSS
    };

    spi_device_handle_t egt2_spi;
    EGT_handle_t EGT2 = {
        .device = &egt2_spi,
        .host = SPI3_HOST,
        .miso = SPI_CLK,
        .clk = SPI_CLK,
        .nss = EGT2_NSS
    };

    spi_device_handle_t egt3_spi;
    EGT_handle_t EGT3 = {
        .device = &egt3_spi,
        .host = SPI3_HOST,
        .miso = SPI_CLK,
        .clk = SPI_CLK,
        .nss = EGT3_NSS
    };

    spi_device_handle_t egt4_spi;
    EGT_handle_t EGT4 = {
        .device = &egt4_spi,
        .host = SPI3_HOST,
        .miso = SPI_CLK,
        .clk = SPI_CLK,
        .nss = EGT4_NSS
    };

    EGT_handle_t EGTs[NUM_OF_EGTS] = {EGT1, EGT2, EGT3, EGT4};// might need to make these pointers maybe

    for(int i = 0; i < NUM_OF_EGTS; i++){
        err = EGT_init(EGTs+i);
        if(err != ESP_OK){
            ESP_LOGE(TAG,"exiting EGT%d initialization error: %d",i+1,err);
            return;
        }
    }

    err = CAN_init(CAN_TX,CAN_RX,CAN_1MBITS);
    if(err != ESP_OK){
        ESP_LOGE(TAG,"exiting CAN initialization error: %d",err);
        return;
    }

    main_ritual_params_t main_params = {
        .ADC_arr = ADCs,
        .ADC_arr_len = NUM_OF_ADCS,
        .EGT_arr = EGTs,
        .EGT_arr_len = NUM_OF_EGTS,
    };

    xTaskCreate(main_ritual,"Main ritual",16384,(void*)&main_params,2,NULL);

#ifdef PRINT_RUN_STATS
    char buffer[512] = {0};
    while(1){
        vTaskGetRunTimeStats(buffer);
        buffer[512] = '\0';
        vTaskDelay(pdMS_TO_TICKS(5000));
        printf("%s\n",buffer);
    }
#endif

}
