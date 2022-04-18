#ifndef NET_SIGNAL_HANDLER_H
#define NET_SIGNAL_HANDLER_H

#include "core/execution_context.h"

class signal_handler
{
public:
    explicit signal_handler(execution_context& ctx);

private:
    boost::asio::signal_set signal_set;
};

#endif
