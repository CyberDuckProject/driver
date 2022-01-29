#ifndef NET_H
#define NET_H

#include "fundamental_types.h"
#include <boost/asio.hpp>
#include <optional>
#include <variant>

namespace net {

class server;

class config
{
public:
    using empty = std::monostate;

    struct manual
    {
        f32 left;
        f32 right;
    };

    template<typename type, typename lambda>
    void handle(lambda&& handler) const
    {
        if (std::holds_alternative<type>(storage))
        {
            handler(std::get<type>(storage));
        }
    }

    void clear();

private:
    friend class server;

    config();
    
    std::variant<empty, manual> storage;
};

enum class sensor_type : u8
{
    water_temperature = 1,
    water_turbidity = 2,
    atmospheric_dust = 3,
    atmospheric_pressure = 4,
    atmospheric_temperature = 5,
    atmospheric_humidity = 6
};

class server
{
public:
    server(u16 port, u16 remote_port);
    ~server();

    bool connected() const;
    config pop_config();

    void send(sensor_type sensor, f32 value);

private:
    struct session
    {
        boost::asio::ip::tcp::socket socket;
        boost::asio::ip::udp::endpoint endpoint;
    };

    void accept();
    void receive();

    boost::asio::io_context ctx;
    std::thread worker;
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::udp::socket sender;
    const u16 remote_port;

    // TODO: add support for different configs
    float data[2];
    std::optional<session> session;
    std::mutex cfg_mutex;
    config cfg;
};

} // namespace net

#endif
