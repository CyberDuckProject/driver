#ifndef NET_MESSAGE_BUFFER_H
#define NET_MESSAGE_BUFFER_H

#include "messages.h"
#include <cassert>
#include <variant>

struct message_buffer
{
public:
    template<typename Message>
    message_buffer& operator=(Message msg)
    {
        header = message_type_of<Message>();
        body = std::move(msg);
        return *this;
    }

    bool empty() const;
    void clear();
    message_type type() const;

    template<typename Message>
    auto& get()
    {
        assert(!empty());
        assert(type() == message_type_of<Message>());
        return std::get<Message>(body);
    }

private:
    message_type header;
    std::variant<std::monostate, control_message, status_message> body{std::monostate{}};
};

#endif
