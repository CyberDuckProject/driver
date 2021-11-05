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

enum class motor
{
	left,
	right
};

void set_speed(motor motor, f32 speed);

f32 water_temperature();

} // namespace io
