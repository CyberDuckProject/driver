#ifndef NET_MESSAGE_BUFFER_H
#define NET_MESSAGE_BUFFER_H

#include "messages.h"
#include <cassert>
#include <variant>

namespace net {

enum class message_type : u64
{
    control = 1,
    status = 2
};

struct message_buffer
{
public:
    template<typename Message>
    message_buffer& operator=(Message msg)
    {
        value = std::move(msg);
        return *this;
    }

    bool empty() const;
    void clear();
    message_type type() const;

    template<typename Message>
    auto& get()
    {
        assert(!empty());
        return std::get<Message>(value);
    }

private:
    std::variant<std::monostate, control_message, status_message> value{std::monostate{}};
};

} // namespace net

#endif
