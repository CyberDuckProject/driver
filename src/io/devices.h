#ifndef IO_DEVICES_H
#define IO_DEVICES_H

#include "utl/fundamental_types.h"
#include <bme280_defs.h>

class esc
{
public:
    explicit esc(u32 broadcom, f64 speed = 0.0);
    ~esc();

    f64 speed() const;
    void set_speed(f64 speed);

private:
    u32 broadcom;
};

constexpr f64 voltage_ratio(f64 r1, f64 r2)
{
    return r2 / (r1 + r2);
}

class mcp3008
{
public:
    mcp3008(bool aux, u32 channel, f64 vref);
    ~mcp3008();

    f64 read_voltage(u32 channel) const;

private:
    f64 vref;
    i32 handle;
};

class bme280
{
public:
    bme280(u32 bus, u32 address);
    ~bme280();

    bme280_data get_sensor_data() const;

private:
    i8 read(u8 reg_addr, u8* reg_data, u32 len);
    i8 write(u8 reg_addr, const u8* reg_data, u32 len);

    i32 handle;
    mutable bme280_dev device;
};

class binary_pin
{
public:
    explicit binary_pin(u32 broadcom, bool on = false);
    ~binary_pin();

    bool state() const;
    void set_state(bool on);

private:
    u32 broadcom;
    bool value;
};

#endif
