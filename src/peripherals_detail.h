#pragma once

#include "fundamental_types.h"
#include <system_error>

namespace peripherals::detail {

class obj_counter
{
protected:
    obj_counter();
    ~obj_counter();

private:
    static u32 count;
};

std::error_code make_gpio_error(i32 ec);
std::error_code make_bme280_error(i32 ec);

} // namespace peripherals::detail
