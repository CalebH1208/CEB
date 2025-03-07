#ifndef CEM_CAN_H
#define CEM_CAN_H

#include "esp_log.h"
#include "driver/twai.h"

#define CAN_ID_OFFSET 0x500

typedef enum{
    CAN_1MBITS,
    CAN_500KBITS,
    CAN_250KBITS,
    CAN_100KBITS,
    CAN_50KBITS
}valid_CAN_speeds_t;

esp_err_t CAN_init(gpio_num_t CAN_tx, gpio_num_t CAN_rx, valid_CAN_speeds_t bus_speed);

esp_err_t CAN_transmit_message(uint32_t ID, uint64_t data);

#endif //CEM_CAN_H