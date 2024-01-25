#ifndef LZCODERS_DETAIL_CORO_DETAIL_HPP
#define LZCODERS_DETAIL_CORO_DETAIL_HPP

#ifndef LZCODERS_CORO_HPP
#error This file is not intended to be used directly. Please, include lzcoders/coro.hpp.
#endif

namespace lzcoders::detail
{
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

    T get_result()
    {
        return result_.get_future().get();
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
    void return_void() { d(); ended_.set_value(true); }

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

    void get_result()
    {
        ended_.get_future().get();
    }
    std::promise<bool> ended_;
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
} // namespace lzcoders::detail
#endif