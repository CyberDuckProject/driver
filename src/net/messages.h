#ifndef NET_MESSAGES_H
#define NET_MESSAGES_H

#include "utl/fundamental_types.h"
#include <boost/asio.hpp>

enum class message_type : u8
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
constexpr message_type message_type_of();

template<>
constexpr message_type message_type_of<control_message>()
{
    return message_type::control;
}

template<>
constexpr message_type message_type_of<status_message>()
{
    return message_type::status;
}

#endif
