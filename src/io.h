#ifndef IO_H
#define IO_H

#include "fundamental_types.h"
#include "io_base.h"

class io : private io_base
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

    struct weather_redout
    {
        f32 pressure;
        f32 temperature;
        f32 humidity;
    };
    weather_redout weather();
};

#endif
