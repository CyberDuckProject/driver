#ifndef NET_MESSAGES_H
#define NET_MESSAGES_H

#include "utl/fundamental_types.h"
#include <boost/asio.hpp>
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
    message_type type() const;

    template<typename AsyncReadStream, typename ReadHandler>
    void async_read(AsyncReadStream& stream, ReadHandler&& handler)
    {
        namespace asio = boost::asio;
        namespace bs = boost::system;

        auto async_read = [&stream](asio::mutable_buffer buffer, auto&& handler) {
            asio::async_read(stream, buffer, std::forward<decltype(handler)>(handler));
        };

        // Read the messag header
        async_read(header(), [this, &handler](const bs::error_code& ec, std::size_t n) {
            if (!ec)
            {
                // Read the message body
                async_read(body(), [this, &handler](const bs::error_code& ec, std::size_t n) {
                    handler(ec, n + sizeof(header));
                })
            }
            else
            {
                handler(ec, n);
            }
        });
    }

private:
    boost::asio::mutable_buffer header();
    boost::asio::mutable_buffer body();

    message_type type;
    std::variant<control_message, status_message> value;
};

#endif
