#include "peripherals.h"

#include "peripheral_error.h"
#include <bme280.h>
#include <cassert>
#include <pigpio.h>
#include <unistd.h>

#define PIGPIO_CALL(call)                                                                          \
    do                                                                                             \
    {                                                                                              \
        if (i32 code = (call); code < 0)                                                           \
        {                                                                                          \
            throw ::peripheral_error{code, false};                                                 \
        }                                                                                          \
    }                                                                                              \
    while (false)
#define BME280_CALL(call)                                                                          \
    do                                                                                             \
    {                                                                                              \
        if (i32 code = (call); code < 0)                                                           \
        {                                                                                          \
            throw ::peripheral_error{code, true};                                                  \
        }                                                                                          \
    }                                                                                              \
    while (false)

namespace {

class pigpio
{
public:
    pigpio()
    {
        // Disable printing
        gpioCfgSetInternals(gpioCfgGetInternals() | PI_CFG_NOSIGHANDLER);
        gpioInitialise();
    }

    ~pigpio()
    {
        gpioTerminate();
    }
} pigpio;

void i2c_delay_us(u32 period, void* intf_ptr)
{
    usleep(period);
}

i8 i2c_read(u8 reg_addr, u8* reg_data, u32 len, void* intf_ptr)
{
    i32 handle = *(i32*)intf_ptr;
    return i2cReadI2CBlockData(handle, reg_addr, (char*)reg_data, len) > 0 ? 0 : -1;
}

i8 i2c_write(u8 reg_addr, const u8* reg_data, u32 len, void* intf_ptr)
{
    i32 handle = *(i32*)intf_ptr;
    return i2cWriteI2CBlockData(handle, reg_addr, (char*)reg_data, len);
}

} // namespace

motor::motor(u32 broadcom) : pin{broadcom}
{
    set_speed(0.0f);
}

motor::~motor()
{
    gpioServo(pin, 1000);
}

void motor::set_speed(f32 value)
{
    assert(value >= 0.0f && value <= 1.0f);
    u32 us = 1000 + static_cast<u32>(value * 1000);
    PIGPIO_CALL(gpioServo(pin, us));
}

f32 motor::speed() const
{
    u32 us;
    PIGPIO_CALL(us = gpioGetServoPulsewidth(pin));
    return static_cast<f32>(us - 1000) / 1000.0f;
}

adc::adc(bool aux, u32 channel, f32 vref) : vref{vref}
{
    PIGPIO_CALL(handle = spiOpen(channel, 1000000, PI_SPI_FLAGS_AUX_SPI(aux)));
}

adc::~adc()
{
    if (handle >= 0)
    {
        spiClose(handle);
    }
}

f32 adc::read(u32 channel) const
{
    assert(channel <= 7);

    char buf[3] = {1, static_cast<char>((8 + channel) << 4), 0};
    char read_buf[3];
    PIGPIO_CALL(spiXfer(handle, buf, read_buf, 3));

    i32 value = ((read_buf[1] << 8) | read_buf[2]) & 0x3FF;
    return static_cast<f32>(value) * vref / 1023.0f;
}

bme280::bme280(u32 bus, u32 address, std::pair<u32, u32> broadcom)
{
    PIGPIO_CALL(gpioSetPullUpDown(broadcom.first, PI_PUD_UP));
    PIGPIO_CALL(gpioSetPullUpDown(broadcom.second, PI_PUD_UP));
    PIGPIO_CALL(handle = i2cOpen(bus, address, 0));

    dev = std::make_unique<bme280_dev>();
    dev->intf = BME280_I2C_INTF;
    dev->intf_ptr = &handle;
    dev->read = i2c_read;
    dev->write = i2c_write;
    dev->delay_us = i2c_delay_us;
    BME280_CALL(bme280_init(dev.get()));

    u8 settings_sel;
    // Recommended mode of operation: Indoor navigation
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
    BME280_CALL(bme280_set_sensor_settings(settings_sel, dev.get()));
    BME280_CALL(bme280_set_sensor_mode(BME280_NORMAL_MODE, dev.get()));

    // First readout is always incorrect
    read();
    usleep(70000);
}

bme280::~bme280()
{
    if (handle >= 0)
    {
        i2cClose(handle);
    }
}

bme280_readout bme280::read() const
{
    bme280_data data;
    BME280_CALL(bme280_get_sensor_data(BME280_ALL, &data, dev.get()));
    
    bme280_readout result;
    result.humidity = data.humidity;
    result.temperature = data.temperature;
    result.pressure = data.pressure;
    return result;
}

output_pin::output_pin(u32 broadcom) : pin{broadcom}
{
    PIGPIO_CALL(gpioSetMode(pin, PI_OUTPUT));
    set_value(false);
}

output_pin::~output_pin()
{
    gpioWrite(pin, 0);
}

void output_pin::set_value(bool on)
{
    PIGPIO_CALL(gpioWrite(pin, on));
}

bool output_pin::value() const
{
    i32 on;
    PIGPIO_CALL(on = gpioRead(pin));
    return on;
}
