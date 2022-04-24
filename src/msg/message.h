#ifndef MSG_MESSAGE_H
#define MSG_MESSAGE_H

#include "utl/fundamental_types.h"

namespace msg {

enum class message_type : u64
{
    control = 1,
    status = 2
};

struct control_message
{
    f64 left;
    f64 right;
};

struct status_message
{
    f64 water_temperature;
    f64 water_turbidity;
    f64 dust;
    f64 battery_voltage;
    f64 pressure;
    f64 temperature;
    f64 humidity;
};

template<typename Message>
constexpr message_type message_type_of;

template<>
constexpr message_type message_type_of<control_message> = message_type::control;

template<>
constexpr message_type message_type_of<status_message> = message_type::status;

} // namespace msg

#endif
