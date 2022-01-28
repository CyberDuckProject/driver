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
            throw ::peripheral_error{code};                                                        \
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

} // namespace

motor::motor(u32 broadcom) : pin{broadcom}
{
    PIGPIO_CALL(gpioServo(pin, 1000));
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
