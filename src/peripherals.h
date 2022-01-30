#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include "fundamental_types.h"
#include <memory>

class motor
{
public:
    explicit motor(u32 broadcom);
    ~motor();

    void set_speed(f32 value);
    f32 speed() const;

private:
    u32 pin;
};

class adc
{
public:
    adc(bool aux, u32 channel, f32 vref);
    ~adc();

    f32 read(u32 channel) const;

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

struct bme280_dev;

class bme280
{
public:
    bme280(u32 bus, u32 address, std::pair<u32, u32> broadcom);
    ~bme280();

    bme280_readout read() const;

private:
    std::pair<u32, u32> pins;
    i32 handle;
    std::unique_ptr<bme280_dev> dev;
};

class output_pin
{
public:
    explicit output_pin(u32 broadcom);
    ~output_pin();

    void set_value(bool on);
    bool value() const;

private:
    u32 pin;
};

#endif
