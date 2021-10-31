#ifndef GPIO_H
#define GPIO_H

#include "fundamental_types.h"
#include <system_error>

namespace gpio {

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

} // namespace gpio

#endif
