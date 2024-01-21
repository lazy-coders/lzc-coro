#pragma once
#include <thread>
#include <lzcoders/debug.hpp>

namespace lzcoders
{
auto io_reader(int fd)
{
    struct async_awaitable
    {
        using is_async_awaitable [[maybe_unused]] = std::true_type;

        bool await_ready()
        {
            d("return false");
            return false;
        }

        void await_suspend(std::coroutine_handle<> h)
        {
            d("Preparing asynchronous reading");
            std::jthread th{
                [action = this->action_, h]{
                    d("Reading some data");
                    if(action)
                    {
                        d("Calling action");
                        action(h);
                    }
                    else
                    {
                        d("Resuming");
                        h.resume();
                    }
                }
            };
            d("Detaching thread");
            th.detach();
            d("Ended work");
        }

        char await_resume()
        {
            d("Returning '+'");
            return '+';
        }

        std::function<void(std::coroutine_handle<>)> action_;
    };

    return async_awaitable{};
}
}