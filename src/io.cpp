#include "io.h"

#include <cassert>
#include <pigpio.h>

#define GPIO_CALL(call)                                                                            \
    do                                                                                             \
    {                                                                                              \
        if (i32 code = call; code < 0)                                                             \
        {                                                                                          \
            throw gpio_error{code};                                                                \
        }                                                                                          \
    }                                                                                              \
    while (false)

namespace io {
namespace {

constexpr u32 left_motor = 13;
constexpr u32 right_motor = 12;

constexpr u32 eyes = 6;
constexpr u32 fan = 7;

constexpr u32 adc_channel = 0;
constexpr u8 temperature = 0;
constexpr u8 turbidity = 1;
i32 adc;

u32 pulse_width(f32 speed)
{
    assert(speed >= 0.0f && speed <= 1.0f);
    return 1000 + static_cast<u32>(speed * 1000.0f);
}

f32 adc_voltage(u8 channel)
{
    assert(channel <= 7);

    char buf[3] = {1, static_cast<char>((8 + channel) << 4), 0};
    char readBuf[3];
    GPIO_CALL(spiXfer(adc, buf, readBuf, 3));

    i32 value = ((readBuf[1] << 8) | readBuf[2]) & 0x3FF;
    return static_cast<f32>(value) * 3.3f / 1023.0f;
}

f32 reverse_divider(f32 output, f32 r1, f32 r2)
{
    f32 scale = r2 / (r1 + r2);
    return output / scale;
}

} // namespace

void init()
{
    gpioCfgSetInternals(gpioCfgGetInternals() | PI_CFG_NOSIGHANDLER); // Disable printing
    GPIO_CALL(gpioInitialise());

    set_left_motor(0.0f);
    set_right_motor(0.0f);

    GPIO_CALL(gpioSetMode(eyes, PI_OUTPUT));
    GPIO_CALL(gpioSetMode(fan, PI_OUTPUT));
    set_eyes(false);
    set_fan(false);

    GPIO_CALL(adc = spiOpen(adc_channel, 1000000, 0));
}

void shutdown()
{
    if (adc > 0)
    {
        spiClose(adc);
    }

    gpioTerminate();
}

void set_left_motor(f32 speed)
{
    GPIO_CALL(gpioServo(left_motor, pulse_width(speed)));
}

void set_right_motor(f32 speed)
{
    GPIO_CALL(gpioServo(right_motor, pulse_width(speed)));
}

void set_eyes(bool on)
{
    GPIO_CALL(gpioWrite(eyes, on));
}

void set_fan(bool on)
{
    GPIO_CALL(gpioWrite(fan, on));
}

f32 water_temperature()
{
    f32 volts = adc_voltage(temperature);
    return (volts - 0.5f) * 100.0f;
}

f32 water_turbidity()
{
    f32 volts = adc_voltage(turbidity);
    f32 x = reverse_divider(volts, 33000, 56000);
    return -1120.4f * x * x + 5742.3f * x - 4352.9f;
}

} // namespace io
