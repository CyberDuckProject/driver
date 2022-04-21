#include "message_buffer.h"

bool message_buffer::empty() const
{
    return std::holds_alternative<std::monostate>(body);
}

void message_buffer::clear()
{
    body = std::monostate{};
}

message_type message_buffer::type() const
{
    assert(!empty());
    return header;
}
