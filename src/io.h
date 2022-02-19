#ifndef IO_H
#define IO_H

#include "peripherals.h"

class io
{
public:
    void set_left_motor(f32 speed);
    f32 left_motor() const;
    
    void set_right_motor(f32 speed);
    f32 right_motor() const;

    void set_eyes(bool on);
    bool eyes() const;

    void set_fan(bool on);
    bool fan() const;

    f32 temperature() const;
    f32 turbidity() const;
    f32 dust() const;
    f32 battery_voltage() const;

    bme280_readout weather() const;

private:
    motor left{13};
    motor right{12};
    output_pin eyes_pin{6};
    output_pin fan_pin{7};
    mcp3008 adc{false, 0, 3.3f};
    ::bme280 bme280{22, 0x76, {19, 21}};
};

#endif
