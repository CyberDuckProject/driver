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

constexpr u32 adc_baud{1000000};
constexpr f32 adc_ref{3.3f};
i32 adc;

constexpr u32 left_motor_gpio{13};
constexpr u32 right_motor_gpio{12};

constexpr u8 temp_channel{0};

f32 read_adc(u8 channel)
{
    assert(channel <= 7);

    char buf[3]{1, static_cast<char>((8 + channel) << 4), 0};
    char readBuf[3];
    GPIO_CALL(spiXfer(adc, buf, readBuf, 3));

    i32 value{((readBuf[1] << 8) | readBuf[2]) & 0x3FF};
    return static_cast<f32>(value) * adc_ref / 1023.0f;
}

} // namespace

void init()
{
    // Disable printing
    u32 internals{gpioCfgGetInternals() | PI_CFG_NOSIGHANDLER};
    gpioCfgSetInternals(internals);

    GPIO_CALL(gpioInitialise());

    set_speed(motor::left, 0.0f);
    set_speed(motor::right, 0.0f);

    GPIO_CALL(adc = spiOpen(0, adc_baud, 0));
}

void shutdown()
{
    if (adc > 0)
    {
        spiClose(adc);
    }

    gpioTerminate();
}

void set_speed(motor motor, f32 speed)
{
    assert(speed >= 0.0f && speed <= 1.0f);
    u32 us{1000 + static_cast<u32>(speed * 1000.0f)};

    u32 gpio{motor == motor::left ? left_motor_gpio : right_motor_gpio};
    GPIO_CALL(gpioServo(gpio, us));
}

f32 water_temperature()
{
    f32 volts{read_adc(temp_channel)};
    return (volts - 0.5f) * 100.0f;
}

} // namespace io
