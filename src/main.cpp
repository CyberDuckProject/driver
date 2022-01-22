#include "io.h"
#define BOOST_LOG_DYN_LINK 1
#include <boost/log/trivial.hpp>

#include <bme280.h>
#include <pigpio.h>

#define GPIO_CALL(call)                                                                            \
    do                                                                                             \
    {                                                                                              \
        if (i32 code = call; code < 0)                                                             \
        {                                                                                          \
            throw io::gpio_error{code};                                                            \
        }                                                                                          \
    }                                                                                              \
    while (false)

i32 bme280;

void delay_us(u32 period, void* intf_ptr)
{
    usleep(period);
}

i8 i2c_read(u8 reg_addr, u8* reg_data, u32 len, void* intf_ptr)
{
    GPIO_CALL(i2cReadI2CBlockData(bme280, reg_addr, (char*)reg_data, len));
    return 0;
}

i8 i2c_write(u8 reg_addr, const u8* reg_data, u32 len, void* intf_ptr)
{
    GPIO_CALL(i2cWriteI2CBlockData(bme280, reg_addr, (char*)reg_data, len));
    return 0;
}

void print_sensor_data(struct bme280_data* comp_data)
{
#ifdef BME280_FLOAT_ENABLE
    printf("%0.2f, %0.2f, %0.2f\r\n", comp_data->temperature, comp_data->pressure,
           comp_data->humidity);
#else
    printf("%ld, %ld, %ld\r\n", comp_data->temperature, comp_data->pressure, comp_data->humidity);
#endif
}

int8_t stream_sensor_data_normal_mode(struct bme280_dev* dev)
{
    int8_t rslt;
    uint8_t settings_sel;
    struct bme280_data comp_data;

    /* Recommended mode of operation: Indoor navigation */
    dev->settings.osr_h = BME280_OVERSAMPLING_1X;
    dev->settings.osr_p = BME280_OVERSAMPLING_16X;
    dev->settings.osr_t = BME280_OVERSAMPLING_2X;
    dev->settings.filter = BME280_FILTER_COEFF_16;
    dev->settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

    settings_sel = BME280_OSR_PRESS_SEL;
    settings_sel |= BME280_OSR_TEMP_SEL;
    settings_sel |= BME280_OSR_HUM_SEL;
    settings_sel |= BME280_STANDBY_SEL;
    settings_sel |= BME280_FILTER_SEL;
    rslt = bme280_set_sensor_settings(settings_sel, dev);
    rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, dev);

    printf("Temperature, Pressure, Humidity\r\n");
    while (1)
    {
        /* Delay while the sensor completes a measurement */
        dev->delay_us(70000, dev->intf_ptr);
        rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
        print_sensor_data(&comp_data);
    }

    return rslt;
}

int main()
{
    io::init();

    // open i2c
    GPIO_CALL(bme280 = i2cOpen(1, 0x77, 0));

    // init driver
    struct bme280_dev dev;
    dev.intf = BME280_I2C_INTF;
    dev.read = i2c_read;
    dev.write = i2c_write;
    dev.delay_us = delay_us;
    i32 rslt = bme280_init(&dev);
    if (rslt != BME280_OK)
    {
        BOOST_LOG_TRIVIAL(error) << "failed to init bme280 (" << rslt << ")";
        return EXIT_FAILURE;
    }

    // stream sensor data
    stream_sensor_data_normal_mode(&dev);

    // close i2c
    i2cClose(bme280);

    io::shutdown();
}
