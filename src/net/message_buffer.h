#ifndef NET_MESSAGE_BUFFER_H
#define NET_MESSAGE_BUFFER_H

#include "messages.h"
#include <boost/asio.hpp>
#include <cassert>
#include <variant>

namespace net {
namespace detail {

struct type_visitor
{
    message_type operator()(const std::monostate&)
    {
        return {};
    }

    template<typename Message>
    message_type operator()(const Message&)
    {
        return message_type_of<Message>;
    }
};

struct data_visitor
{
    boost::asio::mutable_buffer operator()(const std::monostate&)
    {
        return {};
    }

    template<typename Message>
    boost::asio::mutable_buffer operator()(Message& msg)
    {
        return boost::asio::buffer(std::addressof(msg), sizeof(msg));
    }
};

} // namespace detail

template<typename... Messages>
class message_buffer
{
public:
    template<typename Message>
    message_buffer& operator=(Message msg)
    {
        value = std::move(msg);
        return *this;
    }

    bool try_emplace(message_type t)
    {
        return (try_emplace_type<Messages>(t) || ...);
    }

    bool empty() const
    {
        return std::holds_alternative<std::monostate>(value);
    }

    message_type type() const
    {
        assert(!empty());
        return std::visit(detail::type_visitor{}, value);
    }

    template<typename Message>
    Message& get()
    {
        assert(!empty());
        assert(type() == message_type_of<Message>);
        return std::get<Message>(value);
    }

    boost::asio::mutable_buffer data()
    {
        assert(!empty());
        return std::visit(detail::data_visitor{}, value);
    }

private:
    template<typename Message>
    bool try_emplace_type(message_type t)
    {
        if (t == message_type_of<Message>)
        {
            *this = Message{};
            return true;
        }

        return false;
    }

    std::variant<std::monostate, Messages...> value{std::monostate{}};
};

} // namespace net

#endif
