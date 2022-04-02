#include "controller.h"

namespace asio = boost::asio;
using tcp = asio::ip::tcp;
using udp = asio::ip::udp;
using error_code = boost::system::error_code;

void controller::listen()
{
    acceptor.async_accept([this](const error_code& ec, tcp::socket s) {
        socket.emplace(std::move(s));
        read();
    });
}

void controller::read()
{
    socket->async_read_some(asio::buffer(data), [this](const error_code& ec, u64 bytes) {
        if (ec == boost::asio::error::eof)
        {
            socket.reset();
            listen();
            return;
        }

        drv.set_left_motor(data[0]);
        drv.set_right_motor(data[1]);
        read();
    });
}

std::optional<udp::endpoint> controller::remote() const
{
    if (!socket.has_value())
    {
        return std::nullopt;
    }

    tcp::endpoint endpoint{socket->remote_endpoint()};
    return udp::endpoint{endpoint.address(), endpoint.port()};
}
