#include "io.h"
#define BOOST_LOG_DYN_LINK 1
#include <boost/log/trivial.hpp>

#include <bme280.h>
#include <pigpio.h>

int main()
{
    io::init();

    u32 flags = 0 | (1 << 5);
    i32 bme280 = spiOpen(1, 2000000, flags);
    BOOST_LOG_TRIVIAL(debug) << "bme280 " << bme280;

    /*u8 dev_addr;

    bme280_dev dev;
    dev.intf_ptr = &dev_addr;
    dev.intf = BME280_SPI_INTF;
    dev.read = spi_read;
    dev.write = spi_write;
    dev.delay_us = delay_us;*/

    spiClose(bme280);

    io::shutdown();
}
