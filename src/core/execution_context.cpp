#include "execution_context.h"

namespace asio = boost::asio;

auto execution_context::get_executor() -> executor_type
{
    return ctx.get_executor();
}

auto execution_context::make_strand() -> strand_type
{
    return asio::make_strand(get_executor());
}

void execution_context::run()
{
    ctx.join();
}
