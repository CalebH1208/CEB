#include "CEM_EGT.h"

esp_err_t EGT_init(EGT_handle_t * handle){
    spi_bus_config_t spi ={
        .miso_io_num = handle->miso,
        .sclk_io_num = handle->clk,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0
    };

    spi_device_interface_config_t device = {0};
    device.address_bits = 0;
    device.clock_source = SPI_CLK_SRC_XTAL;
    device.mode = 0; // could be wrong
    device.clock_speed_hz = SPI_BUS_SPEED; // probably 5000000; 
    device.spics_io_num = handle->nss;
    device.queue_size = 16;

    esp_err_t check = Spi_initialize_device(&spi,&device,handle->host, handle->device, NULL);
    if(check != ESP_OK)return ESP_ERR_INVALID_STATE;

    return ESP_OK;
}

esp_err_t EGT_sample(EGT_handle_t * handle, float* data){
    uint32_t raw_data = 0;
    esp_err_t err = Spi_read_EGT(handle->device,&raw_data);
    if(err != ESP_OK)return err;
    int sign = (1 << 31) | raw_data;
    int temp_data = (EGT_TEMP_MASK) & (raw_data >> 16);
    float final_value = sign * (((float)temp_data) * 0.25);

    *data = final_value;
    return err;
}