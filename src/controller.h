#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "io_driver.h"
#include <boost/asio.hpp>

class controller
{
public:
    template<typename Executor>
    controller(Executor& ctx, io_driver& drv, u16 port) :
        acceptor{ctx, {boost::asio::ip::tcp::v4(), port}}, drv{drv}
    {
    }

    void listen();
    std::optional<boost::asio::ip::udp::endpoint> remote() const;

private:
    void read();

    boost::asio::ip::tcp::acceptor acceptor;
    io_driver& drv;
    std::optional<boost::asio::ip::tcp::socket> socket;
    f64 data[2];
};

#endif
