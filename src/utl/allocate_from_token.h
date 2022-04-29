#ifndef UTL_ALLOCATE_FROM_TOKEN_H
#define UTL_ALLOCATE_FROM_TOKEN_H

#include <boost/asio.hpp>

namespace utl {

template<typename T, typename Signature, typename CompletionToken, typename... Args>
std::shared_ptr<T> allocate_from_token(CompletionToken& token, Args&&... args)
{
    boost::asio::async_completion<CompletionToken, Signature> init{token};
    auto allocator{boost::asio::get_associated_allocator(init.completion_handler)};
    return std::allocate_shared<T>(allocator, std::forward<Args>(args)...);
}

} // namespace utl

#endif
