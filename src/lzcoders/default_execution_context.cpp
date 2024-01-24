#include <coroutine>
#include <lzcoders/coro.hpp>
#include <lzcoders/debug.hpp>

void lzcoders::default_execution_context::resume(std::coroutine_handle<> h)
{
    d();
    h.resume();
}
