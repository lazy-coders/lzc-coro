#pragma once
#include <optional>
#include <thread>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <deque>
#include <atomic>
#include <memory>

#include <lzcoders/debug.hpp>
/******************************************************************************************
* THREAD POOL                                                                             *
******************************************************************************************/

namespace lzcoders
{

template<std::size_t Size>
struct thread_pool
{
    std::array<std::unique_ptr<std::jthread>, Size> threads_;
    std::deque<std::function<void()>> tasks_;
    std::mutex tasks_mux_;
    std::condition_variable tasks_cond_;
    std::atomic<bool> run = true;
    std::atomic<std::size_t> ended{0};

    thread_pool()
    {
        d("Creating thread pool with {} threads", Size);
        for (std::size_t i = 0; i < Size; i++)
            threads_[i] = std::make_unique<std::jthread>([this]{
                d("Start task executor");
                while(run)
                {
                    d("Getting task");
                    auto task = [this] -> std::optional<std::function<void()>> {
                        std::unique_lock lock{tasks_mux_};
                        tasks_cond_.wait(lock, [this]{ return tasks_.size() > 0 || !run; });
                        if (!run) return{};
                        auto task = tasks_.front();
                        tasks_.pop_front();
                        return task;
                    }();
                    if(task)
                    {
                        d("Executing task");
                        (*task)();
                        d("Task executed");
                    }
                }
                d("Task executor ended");
                ended++;
            });
    }

    ~thread_pool()
    {
        {
            d("Ending thread pool");
            std::lock_guard lock{tasks_mux_};
            run = false;
            tasks_cond_.notify_all();
        }
        d("Waiting executors to end");
        while(ended != Size)
            ;
    }

    void push(std::function<void()> f)
    {
        d();
        std::lock_guard lock{tasks_mux_};
        tasks_.push_back(f);
        tasks_cond_.notify_one();
    }
};
}