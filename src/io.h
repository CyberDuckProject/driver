#pragma once

#include "peripherals.h"

class io
{
public:
    io();
    ~io();
    static io& get();

    void set_left_motor(f32 speed);
    f32 left_motor() const;

    void set_right_motor(f32 speed);
    f32 right_motor() const;

    void set_eyes(bool on);
    bool eyes() const;

    void set_fan(bool on);
    bool fan() const;

    void set_heartbeat(bool on);
    bool heartbeat() const;

    f32 temperature();
    f32 turbidity();
    f32 dust();
    f32 battery_voltage();

    peripherals::bme280_readout weather();

private:
    peripherals::motor left{13};
    peripherals::motor right{12};
    peripherals::output_pin eyes_pin{6};
    peripherals::output_pin fan_pin{7};
    peripherals::output_pin heartbeat_pin{4};
    peripherals::output_pin dust_pin{5};
    peripherals::mcp3008 adc{false, 0, 3.3f};
    peripherals::bme280 bme280{22, 0x76, {19, 21}};
};
