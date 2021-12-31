#include "net.h"

#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <optional>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;
using udp = asio::ip::udp;

extern std::mutex m;

namespace net {

void config::clear()
{
    storage = empty{};
}

config::config() : storage{empty{}}
{
}

server::server(u16 port, u16 remote_port) :
    acceptor{ctx, {tcp::v4(), port}}, sender{ctx, udp::v4()}, remote_port{remote_port}
{
    worker = std::thread{[&]() {
        accept();
        ctx.run();
    }};
}

server::~server()
{
    if (connected())
    {
        session->socket.close();
    }

    ctx.stop();
    worker.join();
}

bool server::connected() const
{
    return session.has_value();
}

config server::pop_config()
{
    std::unique_lock lock{cfg_mutex};
    config copy{cfg};
    cfg.clear();
    return copy;
}

void server::accept()
{
    acceptor.async_accept([&](boost::system::error_code ec, tcp::socket socket) {
        if (!ec)
        {
            BOOST_LOG_TRIVIAL(info) << "connected to " << socket.remote_endpoint();
            udp::endpoint endpoint{socket.remote_endpoint().address(), remote_port};
            session = {std::move(socket), endpoint};
            receive();
        }
        else
        {
            throw boost::system::system_error{ec};
        }
    });
}

void server::receive()
{
    assert(connected());

    // TODO: add support for different configs
    session->socket.async_read_some(asio::buffer(data),
                                    [&](const boost::system::error_code& ec, std::size_t bytes) {
        if (!ec)
        {
            {
                BOOST_LOG_TRIVIAL(debug) << bytes << ' ' << data[0] << ' ' << data[1];
                std::unique_lock lock{cfg_mutex};
                cfg.storage = net::config::manual{data[0], data[1]};
            }
            receive();
        }
        else if (ec == asio::error::eof)
        {
            BOOST_LOG_TRIVIAL(info) << "disconnected";
            session.reset();
            {
                std::unique_lock lock{cfg_mutex};
                cfg.clear();
            }
            accept();
        }
        else
        {
            throw boost::system::system_error{ec};
        }
    });
}

} // namespace net
