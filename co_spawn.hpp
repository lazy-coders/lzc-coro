#pragma once

#include "thread_pool.hpp"
#include "coro.hpp"
#include "d.hpp"

namespace lzcoders
{

template<std::size_t Size, typename T>
auto co_spawn(thread_pool<Size>& tp, task<T> t)
{
    struct res
    {
        std::future<T> f_;
        T operator()()
        {
            if constexpr (not std::is_same_v<T, void>)
            {
                d("waiting for the result");
                return f_.get();
            }
        }
    };

    struct thread_pool_execution_context : coro_execution_context
    {
        thread_pool_execution_context(thread_pool<Size>& tp) : tp_{tp} {}
        void resume(std::coroutine_handle<> h) override
        {
            d("Preparing the 'resume' as new task in the pool");
            tp_.push([h]{
                d("Resuming task from context");
                h.resume(); 
            });
            d("Pushed");
        }

        thread_pool<Size>& tp_;
    };

    t.promise().context_ = std::make_shared<thread_pool_execution_context>(tp);
    d("Pusing the task");
    tp.push([t]{
        d("Resuming task");
        t.resume();
    });

    d("Returning the result reader");
    if constexpr (not std::is_same_v<T, void>)
        return res{t.get_future()};
}

}