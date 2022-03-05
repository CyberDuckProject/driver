#pragma once

#include "peripherals_detail.h"
#include <memory>

struct bme280_dev;

namespace peripherals {

class motor : detail::obj_counter
{
public:
    explicit motor(u32 broadcom);
    ~motor();

    void set_speed(f32 value);
    [[nodiscard]] f32 speed() const;

private:
    u32 pin;
};

class mcp3008 : detail::obj_counter
{
public:
    mcp3008(bool aux, u32 channel, f32 vref);
    ~mcp3008();

    [[nodiscard]] f32 read(u32 channel);

private:
    f32 vref;
    i32 handle;
};

struct bme280_readout
{
    f32 pressure;
    f32 temperature;
    f32 humidity;
};

class bme280 : detail::obj_counter
{
public:
    bme280(u32 bus, u32 address, std::pair<u32, u32> broadcom);
    ~bme280();

    [[nodiscard]] bme280_readout read();

private:
    std::pair<u32, u32> pins;
    std::unique_ptr<bme280_dev> dev;
    i32 handle;
};

class output_pin : detail::obj_counter
{
public:
    explicit output_pin(u32 broadcom, bool on = false);
    ~output_pin();

    void set_value(bool on);
    [[nodiscard]] bool value() const;

private:
    u32 pin;
    bool state;
};

} // namespace peripherals
