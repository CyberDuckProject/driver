#ifndef NETWORK_H
#define NETWORK_H

#include "common_headers.h"
#include <boost/asio.hpp>
#include <optional>

class signal_handler
{
public:
    signal_handler(boost::asio::thread_pool& ctx);

private:
    boost::asio::signal_set signal_set;
};

struct control_message
{
    f64 left_motor;
    f64 right_motor;
};

class connection_manager
{
public:
    connection_manager(boost::asio::thread_pool& ctx, u16 port);

private:
    void accept();
    void read();

    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket socket;
    control_message msg[1];
};

#endif
