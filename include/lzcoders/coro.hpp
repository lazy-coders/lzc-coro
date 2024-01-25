#ifndef LZCODERS_CORO_HPP
#define LZCODERS_CORO_HPP
#include <coroutine>
#include <future>
#include <optional>
#include <lzcoders/debug.hpp>

namespace lzcoders
{
namespace detail {
template<typename T> struct promise;
}

struct coro_execution_context
{
    virtual ~coro_execution_context() {}
    virtual void resume(std::coroutine_handle<> h) = 0;
};

struct default_execution_context : coro_execution_context
{
    void resume(std::coroutine_handle<> h) override;
};

template<typename T>
concept AsyncAwaitable = requires(T t)
{
    typename T::is_async_awaitable;
};

/******************************************************************************************
* TASKS                                                                                   *
******************************************************************************************/
template<typename T>
struct task : std::coroutine_handle<detail::promise<T>>
{
    using promise_type = detail::promise<T>;
 
    T operator()(const std::source_location& sl = std::source_location::current())
    {
        d("called from {}", sl);
        d("Resuming this task");
        this->resume();
        d("Returning the result");
        return this->promise().get_result();
    }

    auto get_future()
    {
        d("Returning the future");
        return this->promise().result_.get_future();
    }

    auto get_result()
    {
        d("Retrieving the result");
        return get_future().get();
    }
};

}

#include <lzcoders/detail/coro_detail.hpp>

#endif