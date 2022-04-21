#ifndef NET_MESSAGES_H
#define NET_MESSAGES_H

#include "utl/fundamental_types.h"
#include <cassert>
#include <variant>

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

struct message_buffer
{
public:
    enum
    {
        invalid_format = 1
    };

    bool empty() const;
    message_type type() const;

    template<message_type Type>
    auto& get()
    {
        assert(!empty());
        assert(type() == Type);

        if constexpr (Type == message_type::control)
        {
            return std::get<control_message>(body);
        }
        else if constexpr (Type == message_type::status)
        {
            return std::get<status_message>(body);
        }
    }

private:
    message_type header;
    std::variant<std::monostate, control_message, status_message> body{std::monostate{}};
};

#endif
