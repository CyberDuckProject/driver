#include "devices.h"

#include "error_codes.h"
#include <bme280.h>
#include <pigpio.h>

#define PIGPIO_VERIFY(expr) VERIFY(expr, make_pigpio_error)
#define BME280_VERIFY(expr) VERIFY(expr, make_bme280_error)
#define VERIFY(expr, fun)                                                                          \
    do                                                                                             \
        if (std::error_code ec{fun(expr)}; ec)                                                     \
            throw std::system_error{ec};                                                           \
    while (false)

namespace detail {

u32 device::count = 0;

device::device()
{
    if (count++ == 0)
    {
        // Disable printing
        u32 internals = gpioCfgGetInternals() | PI_CFG_NOSIGHANDLER;
        gpioCfgSetInternals(internals);

        // Disable unused features
        gpioCfgInterfaces(PI_DISABLE_FIFO_IF | PI_DISABLE_SOCK_IF | PI_DISABLE_ALERT);

        // Initialize pigpio
        PIGPIO_VERIFY(gpioInitialise());
    }
}

device::~device()
{
    if (--count == 0)
    {
        BOOST_LOG_TRIVIAL(debug) << "releasing " << PI_LOCKFILE;
        gpioTerminate();
    }
}

} // namespace detail

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
    // The MCP3008 chip has 8 channels
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

struct bme280::device : bme280_dev
{
};

bme280::bme280(u32 bus, u32 address) : dev{std::make_unique<bme280::device>()}
{
    // Open I2C bus
    BME280_VERIFY(handle = i2cOpen(bus, address, 0));

    // Setup BME280 interface
    dev->intf = BME280_I2C_INTF;
    dev->intf_ptr = this;
    dev->read = [](u8 reg_addr, u8* reg_data, u32 len, void* intf_ptr) {
        assert(intf_ptr);
        return reinterpret_cast<bme280*>(intf_ptr)->read(reg_addr, reg_data, len);
    };
    dev->write = [](u8 reg_addr, const u8* reg_data, u32 len, void* intf_ptr) {
        assert(intf_ptr);
        return reinterpret_cast<bme280*>(intf_ptr)->write(reg_addr, reg_data, len);
    };
    dev->delay_us = [](u32 period, void*) {
        usleep(period);
    };

    // Initialize the device
    BME280_VERIFY(bme280_init(dev.get()));

    // Set sensor settings
    dev->settings.osr_h = BME280_OVERSAMPLING_1X;
    dev->settings.osr_p = BME280_OVERSAMPLING_16X;
    dev->settings.osr_t = BME280_OVERSAMPLING_2X;
    dev->settings.filter = BME280_FILTER_COEFF_16;
    dev->settings.standby_time = BME280_STANDBY_TIME_62_5_MS;
    BME280_VERIFY(bme280_set_sensor_settings(BME280_ALL_SETTINGS_SEL, dev.get()));

    // Set sensor mode
    BME280_VERIFY(bme280_set_sensor_mode(BME280_NORMAL_MODE, dev.get()));
}

bme280::~bme280()
{
    if (handle >= 0)
    {
        i2cClose(handle);
    }
}

weather_data bme280::get_sensor_data() const
{
    bme280_data data;
    BME280_VERIFY(bme280_get_sensor_data(BME280_ALL, &data, dev.get()));

    weather_data result;
    result.humidity = data.humidity;
    result.temperature = data.temperature;
    result.pressure = data.pressure;
    return result;
}

i8 bme280::read(u8 reg_addr, u8* reg_data, u32 len)
{
    return i2cReadI2CBlockData(handle, reg_addr, (char*)reg_data, len) == len ? 0 : -1;
}

i8 bme280::write(u8 reg_addr, const u8* reg_data, u32 len)
{
    return i2cWriteI2CBlockData(handle, reg_addr, (char*)reg_data, len) == 0 ? 0 : -1;
}

motor::motor(u32 broadcom, f64 speed) : broadcom{broadcom}
{
    set_speed(speed);
}

motor::~motor()
{
    gpioServo(broadcom, 1000);
}

f64 motor::speed() const
{
    i32 us;
    PIGPIO_VERIFY(us = gpioGetServoPulsewidth(broadcom));
    return static_cast<f64>(us - 1000) / 1000.0;
}

void motor::set_speed(f64 speed)
{
    assert(speed >= 0.0 && speed <= 1.0);
    u32 us = 1000 + static_cast<u32>(speed * 1000);
    PIGPIO_VERIFY(gpioServo(broadcom, us));
}

binary_out::binary_out(u32 broadcom, bool on) : broadcom{broadcom}
{
    PIGPIO_VERIFY(gpioSetMode(broadcom, PI_OUTPUT));
    set_state(on);
}

binary_out::~binary_out()
{
    gpioWrite(broadcom, false);
}

bool binary_out::state() const
{
    return value;
}

void binary_out::set_state(bool on)
{
    value = on;
    PIGPIO_VERIFY(gpioWrite(broadcom, on));
}
