#ifndef TIMED_LOOP_H
#define TIMED_LOOP_H

#include <boost/asio.hpp>

template<typename F, typename Duration>
class timed_loop
{
public:
    using duration_type = Duration;

    template<typename Executor>
    timed_loop(Executor& ctx, const duration_type& period, F&& fun) :
        timer{ctx}, period{period}, loop{std::forward<F>(fun)}
    {
    }

    void run()
    {
        timer.expires_from_now(std::chrono::steady_clock::duration::zero());
        timer.async_wait([this](const boost::system::error_code& ec) {
            handler(ec);
        });
    }

private:
    void handler(const boost::system::error_code& ec)
    {
        if (!ec)
        {
            loop();
            timer.expires_at(timer.expiry() + period);
            timer.async_wait([this](const boost::system::error_code& ec) {
                handler(ec);
            });
        }
    }

    boost::asio::steady_timer timer;
    Duration period;
    F loop;
    std::atomic_bool running{false};
};

#endif
