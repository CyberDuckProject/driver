#ifndef STATUS_LOOP_H
#define STATUS_LOOP_H

#include "io.h"
#include <boost/asio.hpp>
#include <optional>

class status_loop
{
public:
    status_loop(boost::asio::thread_pool& ctx, io& io, std::chrono::milliseconds period);

    void connect(const boost::asio::ip::udp::endpoint& endpoint);

private:
    void wait_handler();
    void loop();

    ::io& io;
    boost::asio::steady_timer timer;
    std::chrono::milliseconds period;
    boost::asio::ip::udp::socket socket;
    std::optional<boost::asio::ip::udp::endpoint> remote;
    std::mutex mutex;
};

#endif
