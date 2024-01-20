#ifndef LZCODERS_CORO_HPP
#define LZCODERS_CORO_HPP
#include <coroutine>
#include <future>
#include <optional>
#include "d.hpp"

namespace lzcoders
{
template<typename T> struct promise;

struct coro_execution_context
{
    virtual ~coro_execution_context() {}
    virtual void resume(std::coroutine_handle<> h) = 0;
};

struct default_execution_context : coro_execution_context
{
    void resume(std::coroutine_handle<> h) override
    {
        d();
        h.resume();
    }
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
struct task : std::coroutine_handle<promise<T>>
{
    using promise_type = promise<T>;
 
    T operator()(const std::source_location& sl = std::source_location::current())
    {
        d("called from {}", sl);
        d("Resuming this task");
        this->resume();
        d("Returning the result");
        return this->promise().result_.get_future().get();
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

template<typename T> struct awaitable;

template<typename T>
struct promise
{
    task<T> get_return_object() { d(); return {task<T>::from_promise(*this)}; }
    std::suspend_always initial_suspend() noexcept { d(); return {}; }
    std::suspend_always final_suspend() noexcept { d(); return {}; }
    void unhandled_exception() { d(); throw; }
    void return_value(T v)
    {
        d();
        result_.set_value(v);
        if (continuation_)
            continuation_->resume();
    }

    template<typename U>
    auto await_transform(task<U> t)
    {
        d();
        t.promise().context_ = context_;
        return awaitable<U>{t};
    }

    template<AsyncAwaitable U>
    auto await_transform(U t)
    {
        d("Updating awaitable action");
        t.action_ = [this](std::coroutine_handle<> h){
            d("Async awaiter complete");
            context_->resume(h);
        };

        return t;
    }
    std::promise<T> result_;
    std::shared_ptr<coro_execution_context> context_ = std::make_shared<default_execution_context>();
    std::optional<std::coroutine_handle<>> continuation_;
};

template<>
struct promise<void>
{
    task<void> get_return_object() { d(); return {task<void>::from_promise(*this)}; }
    std::suspend_always initial_suspend() noexcept { d(); return {}; }
    std::suspend_always final_suspend() noexcept { d(); return {}; }
    void unhandled_exception() { d(); throw; }
    void return_void() { d(); }

    template<typename U>
    auto await_transform(task<U> t)
    {
        d();
        return awaitable<U>{t};
    }

    template<AsyncAwaitable U>
    auto await_transform(U t)
    {
        t.action_ = [this](std::coroutine_handle<> h){
            d("Async awaiter complete");
            context_->resume(h);
        };

        return t;
    }
    std::shared_ptr<coro_execution_context> context_ = std::make_shared<default_execution_context>();
};

template<typename T>
struct awaitable
{
    bool await_ready()
    {
        d(); 
        return false; 
    }

    void await_suspend(std::coroutine_handle<> h)
    {
        d();
        t_.promise().continuation_ = h;
        t_.resume();
    }

    T await_resume()
    {
        d("Returns the inner task result");
        return t_.get_result();
    }

    task<T> t_;
};
}

#endif