#include "message_buffer.h"

bool message_buffer::empty() const
{
    return std::holds_alternative<std::monostate>(value);
}

void message_buffer::clear()
{
    value = std::monostate{};
}

message_type message_buffer::type() const
{
    assert(!empty());

    if (std::holds_alternative<control_message>(value))
    {
        return message_type::control;
    }
    else if (std::holds_alternative<status_message>(value))
    {
        return message_type::status;
    }
}
