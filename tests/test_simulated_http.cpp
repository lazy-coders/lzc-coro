#include <lzcoders/debug.hpp>
#include "thread_pool.hpp"
#include <lzcoders/coro.hpp>
#include "io_reader.hpp"
#include "co_spawn.hpp"
#include <gtest/gtest.h>
constexpr std::size_t PoolSize = 1;
std::atomic<int> tasks = 1;

lzcoders::task<long long> parse_http_headers(int client_socket)
{
    d("Start parse_http_headers");
    auto r = co_await lzcoders::io_reader(client_socket);
    d("Ended parse_http_headers");
    co_return r;
}

lzcoders::task<void> process_http_request(int client_socket)
{
    d("Start process_http_request");
    /*auto r = */co_await parse_http_headers(client_socket);
    d("Ended process_http_request");
    std::cout << "Request processed" << std::endl;
    tasks --;
    co_return /*r*/;
}

lzcoders::task<void> http_acceptor_loop(lzcoders::thread_pool<PoolSize>& pool)
{
    for (int i = 0; i < 10; ++i)
    {
        d("Received petition");
        // Simulation of a asynchronous accept
        auto client_socket = co_await lzcoders::io_reader(1);

        auto process = process_http_request(client_socket);
        tasks ++;
        lzcoders::co_spawn(pool, process);
        d("Petition queued");
    }

    tasks --;
    co_return /*'a'*/;
}

TEST(lzc_coro, simulated_http)
{
    lzcoders::thread_pool<PoolSize> pool;

    lzcoders::co_spawn(pool, http_acceptor_loop(pool));

    while(tasks > 0)
        ;
}