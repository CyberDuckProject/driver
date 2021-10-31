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

} // namespace gpio

#endif
