#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include "io_driver.h"
#include <boost/asio.hpp>

class transmitter
{
public:
    using endpoint_type = boost::asio::ip::udp::endpoint;

    template<typename Executor>
    explicit transmitter(Executor& ctx, io_driver& drv) :
        socket{ctx, boost::asio::ip::udp::v4()}, drv{drv}
    {
    }

    void send_to(const endpoint_type& remote);

private:
    boost::asio::ip::udp::socket socket;
    io_driver& drv;
};

#endif
