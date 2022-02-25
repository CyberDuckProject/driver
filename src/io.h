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

    void set_heartbeat(bool on);
    bool heartbeat() const;

    f32 temperature();
    f32 turbidity();
    f32 dust();
    f32 battery_voltage();

    bme280_readout weather();

private:
    motor left{13};
    motor right{12};
    output_pin eyes_pin{6};
    output_pin fan_pin{7};
    output_pin heartbeat_pin{4};
    output_pin dust_pin{5};
    mcp3008 adc{false, 0, 3.3f};
    ::bme280 bme280{22, 0x76, {19, 21}};
};

#endif
