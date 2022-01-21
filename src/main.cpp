#include "io.h"
#define BOOST_LOG_DYN_LINK 1
#include <boost/log/trivial.hpp>

#include <bme280.h>
#include <pigpio.h>

i32 bme280;

void delay_us(u32 period, void* intf_ptr)
{
    usleep(period);
}

i8 spi_read(u8 reg_addr, u8* reg_data, u32 len, void* intf_ptr)
{
    // write reg addr
    spiWrite(bme280, (char*)&reg_addr, sizeof(reg_addr));
    // read data
    spiRead(bme280, (char*)reg_data, len);

    return 0;
}

i8 spi_write(u8 reg_addr, const u8* reg_data, u32 len, void* intf_ptr)
{
    // write reg addr
    spiWrite(bme280, (char*)&reg_addr, sizeof(reg_addr));
    // write data
    spiWrite(bme280, (char*)reg_data, len);

    return 0;
}

void print_sensor_data(struct bme280_data* comp_data)
{
    float temp, press, hum;

#ifdef BME280_FLOAT_ENABLE
    temp = comp_data->temperature;
    press = 0.01 * comp_data->pressure;
    hum = comp_data->humidity;
#else
#ifdef BME280_64BIT_ENABLE
    temp = 0.01f * comp_data->temperature;
    press = 0.0001f * comp_data->pressure;
    hum = 1.0f / 1024.0f * comp_data->humidity;
#else
    temp = 0.01f * comp_data->temperature;
    press = 0.01f * comp_data->pressure;
    hum = 1.0f / 1024.0f * comp_data->humidity;
#endif
#endif
    printf("%0.2lf deg C, %0.2lf hPa, %0.2lf%%\n", temp, press, hum);
}

int8_t stream_sensor_data_forced_mode(struct bme280_dev* dev)
{
    /* Variable to define the result */
    int8_t rslt = BME280_OK;

    /* Variable to define the selecting sensors */
    uint8_t settings_sel = 0;

    /* Variable to store minimum wait time between consecutive measurement in force mode */
    uint32_t req_delay;

    /* Structure to get the pressure, temperature and humidity values */
    struct bme280_data comp_data;

    /* Recommended mode of operation: Indoor navigation */
    dev->settings.osr_h = BME280_OVERSAMPLING_1X;
    dev->settings.osr_p = BME280_OVERSAMPLING_16X;
    dev->settings.osr_t = BME280_OVERSAMPLING_2X;
    dev->settings.filter = BME280_FILTER_COEFF_16;

    settings_sel =
        BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

    /* Set the sensor settings */
    rslt = bme280_set_sensor_settings(settings_sel, dev);
    if (rslt != BME280_OK)
    {
        fprintf(stderr, "Failed to set sensor settings (code %+d).", rslt);

        return rslt;
    }

    printf("Temperature, Pressure, Humidity\n");

    /*Calculate the minimum delay required between consecutive measurement based upon the sensor
     * enabled and the oversampling configuration. */
    req_delay = bme280_cal_meas_delay(&dev->settings);

    /* Continuously stream sensor data */
    while (1)
    {
        /* Set the sensor to forced mode */
        rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);
        if (rslt != BME280_OK)
        {
            fprintf(stderr, "Failed to set sensor mode (code %+d).", rslt);
            break;
        }

        /* Wait for the measurement to complete and print data */
        dev->delay_us(req_delay, dev->intf_ptr);
        rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
        if (rslt != BME280_OK)
        {
            fprintf(stderr, "Failed to get sensor data (code %+d).", rslt);
            break;
        }

        print_sensor_data(&comp_data);
    }

    return rslt;
}

int main()
{
    io::init();

    // init spi
    u32 flags = 0 | (1 << 8);
    bme280 = spiOpen(0, 2000000, flags);
    BOOST_LOG_TRIVIAL(debug) << "bme280: " << bme280;

    // init driver
    u8 dev_addr;
    i8 result = BME280_OK;
    bme280_dev dev;
    dev.intf_ptr = &dev_addr;
    dev.intf = BME280_SPI_INTF;
    dev.read = spi_read;
    dev.write = spi_write;
    dev.delay_us = delay_us;
    result = bme280_init(&dev);
    BOOST_LOG_TRIVIAL(debug) << "result: " << result;

    stream_sensor_data_forced_mode(&dev);

    // shutdown spi
    spiClose(bme280);

    io::shutdown();
}
