#include "net/connection.h"
#include "net/signal_handler.h"

namespace asio = boost::asio;

msg::message_buffer<msg::control_message> buffer;

template<typename ExecutionContext>
void read(net::connection<ExecutionContext>& conn);

template<typename ExecutionContext>
void accept(net::connection<ExecutionContext>& conn)
{
    conn.async_accept([&](...) {
        BOOST_LOG_TRIVIAL(debug) << "connected";
        read(conn);
    });
}

template<typename ExecutionContext>
void read(net::connection<ExecutionContext>& conn)
{
    conn.async_read_message(buffer, [&](boost::system::error_code ec, std::size_t) {
        if (!ec)
        {
            auto& m = buffer.get<msg::control_message>();
            BOOST_LOG_TRIVIAL(debug) << m.left << " " << m.right;
            read(conn);
        }
        else
        {
            BOOST_LOG_TRIVIAL(debug) << ec.message();
            BOOST_LOG_TRIVIAL(debug) << "disconnected";
            accept(conn);
        }
    });
}

void guarded_main()
{
    BOOST_LOG_TRIVIAL(info) << "starting 4 threads";
    asio::thread_pool ctx{4};

    BOOST_LOG_TRIVIAL(info) << "registering signal handler";
    net::signal_handler signal_handler{ctx};

    BOOST_LOG_TRIVIAL(info) << "listening on port 1333";
    net::connection connection{ctx, 1333};

    accept(connection);

    ctx.join();
}

i32 main()
{
    try
    {
        guarded_main();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        BOOST_LOG_TRIVIAL(fatal) << e.what();
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << "unknown error";
    }

    return EXIT_FAILURE;
}
