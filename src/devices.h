#ifndef DEVICES_H
#define DEVICES_H

#include "common_headers.h"

namespace detail {

class device
{
protected:
    device();
    ~device();

private:
    static u32 count;
};

} // namespace detail

class mcp3008 : private detail::device
{
public:
    mcp3008(bool aux, u32 channel, f64 vref);
    ~mcp3008();

    f64 read_voltage(u32 channel) const;

private:
    f64 vref;
    i32 handle;
};

struct weather_data
{
    f64 pressure;
    f64 temperature;
    f64 humidity;
};

class bme280 : private detail::device
{
public:
    bme280(u32 bus, u32 address);
    ~bme280();

    weather_data get_sensor_data() const;

private:
    struct device;

    i8 read(u8 reg_addr, u8* reg_data, u32 len);
    i8 write(u8 reg_addr, const u8* reg_data, u32 len);

    i32 handle;
    std::unique_ptr<device> dev;
};

class motor : private detail::device
{
public:
    explicit motor(u32 broadcom, f64 speed = 0.0);
    ~motor();

    f64 speed() const;
    void set_speed(f64 speed);

private:
    u32 broadcom;
};

class binary_out : private detail::device
{
public:
    explicit binary_out(u32 broadcom, bool on = false);
    ~binary_out();

    bool state() const;
    void set_state(bool on);

private:
    u32 broadcom;
    bool value;
};

#endif
