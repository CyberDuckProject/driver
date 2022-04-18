#ifndef CORE_EXECUTION_CONTEXT_H
#define CORE_EXECUTION_CONTEXT_H

#include <boost/asio.hpp>

class execution_context
{
public:
    using executor_type = boost::asio::thread_pool::executor_type;
    using strand_type = boost::asio::strand<boost::asio::thread_pool::executor_type>;

    executor_type get_executor();
    strand_type make_strand();

    void run();
    void stop();

private:
    boost::asio::thread_pool ctx{4};
};

#endif
