#ifndef NET_MESSAGE_BUFFER_H
#define NET_MESSAGE_BUFFER_H

#include "messages.h"
#include <cassert>
#include <variant>

namespace net {
namespace detail {

template<typename T, typename... Ts>
constexpr bool contains = (std::is_same_v<T, Ts> || ...);

} // namespace detail

template<typename... Messages>
class message_buffer
{
public:
    message_buffer() = default;

    template<typename Message>
    explicit message_buffer(Message msg)
    {
    }

    template<typename Message>
    message_buffer& operator=(Message msg)
    {
    }

    bool empty() const
    {
    }

    void clear()
    {
    }

    const message_type& type() const
    {
    }

    template<typename Message>
    auto& get()
    {
    }

    template<typename Message>
    const auto& get() const
    {
    }

private:
    message_type msg_type;
    std::variant<std::monostate, Messages...> value{std::monostate{}};
};

} // namespace net

#endif
