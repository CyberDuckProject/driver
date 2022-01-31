#ifndef IO_H
#define IO_H

#include "peripherals.h"

class io
{
public:
    void set_left_motor(f32 speed);
    void set_right_motor(f32 speed);

    void set_eyes(bool on);
    void set_fan(bool on);

    f32 temperature() const;
    f32 turbidity() const;
    f32 dust() const;

    bme280_readout weather() const;

private:
    motor left{13};
    motor right{12};
    output_pin eyes{6};
    output_pin fan{7};
    ::adc adc{false, 0, 3.3f};
    ::bme280 bme280{22, 0x76, {19, 21}};
};

#endif
