#ifndef NET_MESSAGE_BUFFER_H
#define NET_MESSAGE_BUFFER_H

#include "messages.h"
#include <cassert>
#include <variant>

namespace net {
namespace detail {

template<typename T, typename... Ts>
constexpr bool contains = (std::is_same_v<T, Ts> || ...);

struct type_visitor
{
    message_type operator()(const std::monostate&)
    {
        return {};
    }

    message_type operator()(const control_message&)
    {
        return message_type::control;
    }

    message_type operator()(const status_message&)
    {
        return message_type::status;
    }
};

} // namespace detail

template<typename... Messages>
class message_buffer
{
public:
    message_buffer() = default;

    template<typename Message>
    explicit message_buffer(const Message& msg)
    {
        *this = msg;
    }

    template<typename Message>
    message_buffer& operator=(const Message& msg)
    {
        static_assert(detail::contains<Message, Messages...>);
        value = msg;
        return *this;
    }

    bool empty() const
    {
        return std::holds_alternative<std::monostate>(value);
    }

    void clear()
    {
        value = std::monostate{};
    }

    message_type type() const
    {
        assert(!empty());
        return std::visit(detail::type_visitor{}, value);
    }

    template<typename Message>
    Message& get()
    {
        assert_get<Message>();
        return std::get<Message>(value);
    }

    template<typename Message>
    const Message& get() const
    {
        assert_get<Message>();
        return std::get<Message>(value);
    }

private:
    template<typename Message>
    void assert_get() const
    {
        static_assert(detail::contains<Message, Messages...>);
        assert(!empty());
        assert(std::holds_alternative<Message>(value));
    }

    std::variant<std::monostate, Messages...> value{std::monostate{}};
};

} // namespace net

#endif
