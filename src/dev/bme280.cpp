#include "bme280.h"

#include <bme280.h>
#include <cassert>
#include <pigpio.h>
#include <unistd.h>

bme280::bme280(u32 bus, u32 address)
{
    // Open I2C bus
    handle = i2cOpen(bus, address, 0);

    // Setup BME280 interface and callbacks
    device.intf = BME280_I2C_INTF;
    device.intf_ptr = this;
    device.read = [](u8 reg_addr, u8* reg_data, u32 len, void* intf_ptr) {
        return static_cast<bme280*>(intf_ptr)->read(reg_addr, reg_data, len);
    };
    device.write = [](u8 reg_addr, const u8* reg_data, u32 len, void* intf_ptr) {
        return static_cast<bme280*>(intf_ptr)->write(reg_addr, reg_data, len);
    };
    device.delay_us = [](u32 period, void*) {
        usleep(period);
    };

    // Initialize the device
    bme280_init(&device);

    // Set sensor settings
    device.settings.osr_h = BME280_OVERSAMPLING_1X;
    device.settings.osr_p = BME280_OVERSAMPLING_16X;
    device.settings.osr_t = BME280_OVERSAMPLING_2X;
    device.settings.filter = BME280_FILTER_COEFF_16;
    device.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;
    bme280_set_sensor_settings(BME280_ALL_SETTINGS_SEL, &device);

    // Set sensor mode
    bme280_set_sensor_mode(BME280_NORMAL_MODE, &device);
}

bme280::~bme280()
{
    if (handle >= 0)
    {
        i2cClose(handle);
    }
}

bme280_data bme280::get_sensor_data() const
{
    bme280_data data;
    bme280_get_sensor_data(BME280_ALL, &data, &device);
    return data;
}

i8 bme280::read(u8 reg_addr, u8* reg_data, u32 len)
{
    return i2cReadI2CBlockData(handle, reg_addr, (char*)reg_data, len) == len ? 0 : -1;
}

i8 bme280::write(u8 reg_addr, const u8* reg_data, u32 len)
{
    return i2cWriteI2CBlockData(handle, reg_addr, (char*)reg_data, len) == 0 ? 0 : -1;
}
