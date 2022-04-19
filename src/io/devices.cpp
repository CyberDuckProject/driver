#include "devices.h"

#include "error_codes.h"
#include <bme280.h>
#include <cassert>
#include <pigpio.h>
#include <unistd.h>

#define BME280_VERIFY(x) VERIFY(make_bme280_error(x))
#define PIGPIO_VERIFY(x) VERIFY(make_pigpio_error(x))
#define VERIFY(x)                                                                                  \
    do                                                                                             \
        if (std::error_code ec{x}; ec.value() < 0)                                                 \
            throw ::std::system_error{ec};                                                         \
    while (false)

esc::esc(u32 broadcom, f64 speed) : broadcom{broadcom}
{
    set_speed(speed);
}

esc::~esc()
{
    gpioServo(broadcom, 1000);
}

f64 esc::speed() const
{
    i32 us;
    PIGPIO_VERIFY(us = gpioGetServoPulsewidth(broadcom));
    return static_cast<f64>(us - 1000) / 1000.0;
}

void esc::set_speed(f64 speed)
{
    assert(speed >= 0.0 && speed <= 1.0);
    u32 us = 1000 + static_cast<u32>(speed * 1000);
    PIGPIO_VERIFY(gpioServo(broadcom, us));
}

mcp3008::mcp3008(bool aux, u32 channel, f64 vref) : vref{vref}
{
    PIGPIO_VERIFY(handle = spiOpen(channel, 1000000, PI_SPI_FLAGS_AUX_SPI(aux)));
}

mcp3008::~mcp3008()
{
    if (handle >= 0)
    {
        spiClose(handle);
    }
}

f64 mcp3008::read_voltage(u32 channel) const
{
    assert(channel <= 7);

    // Single-ended channel selection
    char tx[3], rx[3];
    tx[0] = 1;
    tx[1] = (8 + channel) << 4;
    PIGPIO_VERIFY(spiXfer(handle, tx, rx, 3));

    // Assemble digital output code
    u32 code = (rx[1] << 8) | rx[2];
    code &= 0x3FF;

    // Calculate input voltage
    return static_cast<f64>(code) * vref / 1023.0;
}

bme280::bme280(u32 bus, u32 address)
{
    // Open I2C bus
    PIGPIO_VERIFY(handle = i2cOpen(bus, address, 0));

    // Configure I2C interface
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

    // Initialize the driver
    BME280_VERIFY(bme280_init(&device));

    // Set sensor settings and mode
    device.settings.osr_h = BME280_OVERSAMPLING_1X;
    device.settings.osr_p = BME280_OVERSAMPLING_16X;
    device.settings.osr_t = BME280_OVERSAMPLING_2X;
    device.settings.filter = BME280_FILTER_COEFF_16;
    device.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;
    BME280_VERIFY(bme280_set_sensor_settings(BME280_ALL_SETTINGS_SEL, &device));
    BME280_VERIFY(bme280_set_sensor_mode(BME280_NORMAL_MODE, &device));
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
    BME280_VERIFY(bme280_get_sensor_data(BME280_ALL, &data, &device));
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

binary_pin::binary_pin(u32 broadcom, bool on) : broadcom{broadcom}
{
    PIGPIO_VERIFY(gpioSetMode(broadcom, PI_OUTPUT));
    set_state(on);
}

binary_pin::~binary_pin()
{
    gpioWrite(broadcom, false);
}

bool binary_pin::state() const
{
    return value;
}

void binary_pin::set_state(bool on)
{
    value = on;
    PIGPIO_VERIFY(gpioWrite(broadcom, on));
}
