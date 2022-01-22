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

i8 spi_read(u8 reg_addr, u8* reg_data, u32 len, void* intf_ptr)
{
    // write reg addr
    GPIO_CALL(spiWrite(bme280, (char*)&reg_addr, 1));
    // read data
    GPIO_CALL(spiRead(bme280, (char*)reg_data, len));

    return 0;
}

i8 spi_write(u8 reg_addr, const u8* reg_data, u32 len, void* intf_ptr)
{
    // write reg addr
    GPIO_CALL(spiWrite(bme280, (char*)&reg_addr, 1));
    // write data
    GPIO_CALL(spiWrite(bme280, (char*)reg_data, len));

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

    // init spi
    GPIO_CALL(bme280 = spiOpen(0, 2000000, PI_SPI_FLAGS_AUX_SPI(1)));

    // init driver
    i32 result = BME280_OK;
    bme280_dev dev;
    dev.intf_ptr = nullptr;
    dev.intf = BME280_SPI_INTF;
    dev.read = spi_read;
    dev.write = spi_write;
    dev.delay_us = delay_us;
    result = bme280_init(&dev);
    if (result < 0)
    {
        BOOST_LOG_TRIVIAL(error) << "failed to init bme280 (" << result << ")";
        return EXIT_FAILURE;
    }

    result = stream_sensor_data_normal_mode(&dev);
    if (result < 0)
    {
        BOOST_LOG_TRIVIAL(error) << "failed to stream sensor data (" << result << ")";
        return EXIT_FAILURE;
    }

    // shutdown spi
    GPIO_CALL(spiClose(bme280));

    io::shutdown();
}
