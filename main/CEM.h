#ifndef CEM_H
#define CEM_H

#include "esp_log.h"

#include "LEDs.h"

#include "CEM_ADC.h"
#include "CEM_CAN.h"
#include "CEM_EGT.h"

#define SAMPLE_N_SEND_HZ 100

typedef struct{
    EGT_handle_t * EGT_arr;
    int EGT_arr_len;
    ADC_handle_t * ADC_arr;
    int ADC_arr_len;
}main_ritual_params_t;

void main_ritual(void * params);

#endif // CEM_H