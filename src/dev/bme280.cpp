#include "bme280.h"

#include <bme280.h>
#include <cassert>
#include <pigpio.h>
#include <unistd.h>

namespace dev {
namespace {

i8 i2c_read(u8 reg_addr, u8* reg_data, u32 len, void* intf_ptr)
{
    assert(intf_ptr);
    i32 handle = *static_cast<i32*>(intf_ptr);
    return i2cReadI2CBlockData(handle, reg_addr, (char*)reg_data, len) == len ? 0 : -1;
}

i8 i2c_write(u8 reg_addr, const u8* reg_data, u32 len, void* intf_ptr)
{
    assert(intf_ptr);
    i32 handle = *static_cast<i32*>(intf_ptr);
    return i2cWriteI2CBlockData(handle, reg_addr, (char*)reg_data, len) == 0 ? 0 : -1;
}

void i2c_delay_us(u32 period, void*)
{
    usleep(period);
}

} // namespace

bme280::bme280(u32 bus, u32 address)
{
    // Open I2C bus
    handle = i2cOpen(bus, address, 0);

    // Setup BME280 interface
    device.intf = BME280_I2C_INTF;
    device.intf_ptr = &handle;
    device.read = i2c_read;
    device.write = i2c_write;
    device.delay_us = i2c_delay_us;

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

} // namespace dev
