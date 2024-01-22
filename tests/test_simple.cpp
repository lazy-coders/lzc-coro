#include <gtest/gtest.h>
#include <lzcoders/coro.hpp>
#include <condition_variable>
#include <memory>
#include <lzcoders/debug.hpp>

using namespace lzcoders;

task<void> notify_at_end(std::shared_ptr<int> counter)
{
    struct guard {
        ~guard()
        {
            d();
            (*counter)++;
        }
        std::shared_ptr<int> counter;
    };
    guard guard_{counter};

    co_return;
}

TEST(lzc_coro, task_with_void_return_should_be_waited_and_completed)
{
    auto counter = std::make_shared<int>(0);
    auto t = notify_at_end(counter);
    t();
    EXPECT_EQ(*counter, 1);
}

task<int> return_same_value(int value)
{
    co_return value;
}

TEST(lzc_coro, value_should_be_returned_correctly)
{
    for(int i = -5; i < 10; i++)
    {
        auto t = return_same_value(i);
        EXPECT_EQ(i, t());
    }
}

task<int> return_double_using_co_await_of_task(int value)
{
    co_return co_await return_same_value(value) + co_await return_same_value(value);
}

TEST(lzc_coro, co_await_of_task_should_work)
{
    for(int i = -5; i < 10; i++)
    {
        auto t = return_double_using_co_await_of_task(i);
        EXPECT_EQ(i+i, t());
    }
}