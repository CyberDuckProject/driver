#ifndef GPIO_ERROR_H
#define GPIO_ERROR_H

#include <system_error>
#include "fundamental_types.h"

class gpio_error : public std::system_error
{
public:
	explicit gpio_error(i32 error_code);
};

#endif
