#include "network.h"

namespace asio = boost::asio;
namespace bs = boost::system;
using asio::ip::tcp;

signal_handler::signal_handler(asio::thread_pool& ctx) : signal_set{ctx, SIGINT, SIGTERM}
{
    signal_set.async_wait([&ctx](const bs::error_code& ec, i32 sig) {
        if (!ec)
        {
            BOOST_LOG_TRIVIAL(debug) << "received signal " << sig;
            ctx.stop();
        }
        else
        {
            throw bs::system_error{ec};
        }
    });
}

connection_manager::connection_manager(asio::thread_pool& ctx, u16 port) : acceptor{ctx, {tcp::v4(), port}}, socket{ctx}
{
    accept();
}

void connection_manager::accept()
{
    acceptor.async_accept([this](const bs::error_code& ec, tcp::socket s) {
        if (!ec)
        {
            BOOST_LOG_TRIVIAL(debug) << "connected to " << s.remote_endpoint();
            socket = std::move(s);
            read();
        }
        else
        {
            throw bs::system_error{ec};
        }
    });
}

void connection_manager::read()
{
    // TODO: add a timeout to detect internet problems
    asio::async_read(socket, asio::buffer(msg), [this](const bs::error_code& ec, std::size_t size) {
        if (!ec)
        {
            // TODO: handle the message
            read();
        }
        else if (ec == asio::error::eof || ec == asio::error::connection_reset)
        {
            BOOST_LOG_TRIVIAL(debug) << "disconnected";
            socket.close();
            accept();
        }
        else
        {
            throw bs::system_error{ec};
        }
    });
}
