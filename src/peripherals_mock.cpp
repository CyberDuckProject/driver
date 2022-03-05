#include "peripherals.h"

#include <bme280.h>
#include <cassert>
#include <random>

namespace peripherals {
namespace {

std::mt19937 rng{std::random_device{}()};

f32 generate(f32 lo, f32 hi)
{
    return std::uniform_real_distribution<f32>{lo, hi}(rng);
}

} // namespace

namespace detail {

base::base()
{
}

base::~base()
{
}

} // namespace detail

motor::motor(u32)
{
}

motor::~motor()
{
}

void motor::set_speed(f32 value)
{
    assert(value >= 0.0f && value <= 1.0f);
    *reinterpret_cast<f32*>(&pin) = value;
}

f32 motor::speed() const
{
    return *reinterpret_cast<const f32*>(&pin);
}

mcp3008::mcp3008(bool, u32, f32 vref) : vref{vref}
{
}

mcp3008::~mcp3008()
{
}

f32 mcp3008::read(u32 channel) const
{
    assert(channel <= 7);
    return generate(0.0f, vref);
}

bme280::bme280(u32, u32, std::pair<u32, u32>)
{
}

bme280::~bme280()
{
}

bme280_readout bme280::read()
{
    bme280_readout result;
    result.humidity = generate(0.0f, 100.0f);
    result.pressure = generate(99000.0f, 110000.0f);
    result.temperature = generate(20.0f, 30.0f);
    return result;
}

output_pin::output_pin(u32, bool)
{
    set_value(false);
}

output_pin::~output_pin()
{
}

void output_pin::set_value(bool on)
{
    state = on;
}

bool output_pin::value() const
{
    return state;
}

} // namespace peripherals
