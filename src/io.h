#pragma once

#include "fundamental_types.h"
#include <system_error>

namespace io {

class gpio_error : public std::system_error
{
public:
    explicit gpio_error(i32 error_code);
};

void init();
void shutdown();

void set_left_motor(f32 speed);
void set_right_motor(f32 speed);

void set_eyes(bool on);
void set_fan(bool on);

f32 water_temperature();
f32 water_turbidity();

} // namespace io
