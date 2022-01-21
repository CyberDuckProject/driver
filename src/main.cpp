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
    // set cs
    gpioWrite(18, 1);
    gpioWrite(18, 0);

    // write reg addr
    spiWrite(bme280, (char*)&reg_addr, sizeof(reg_addr));
    // read data
    spiRead(bme280, (char*)reg_data, len);

    // set cs
    gpioWrite(18, 1);

    return 0;
}

i8 spi_write(u8 reg_addr, const u8* reg_data, u32 len, void* intf_ptr)
{
    // set cs
    gpioWrite(18, 1);
    gpioWrite(18, 0);

    // write reg addr
    spiWrite(bme280, (char*)&reg_addr, sizeof(reg_addr));
    // write data
    spiWrite(bme280, (char*)reg_data, len);

    // set cs
    gpioWrite(18, 1);

    return 0;
}

int main()
{
    io::init();

    // init spi
    u32 flags = 0 | (1 << 5);
    bme280 = spiOpen(1, 2000000, flags);
    gpioSetMode(18, PI_OUTPUT);
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

    // shutdown spi
    spiClose(bme280);

    io::shutdown();
}
