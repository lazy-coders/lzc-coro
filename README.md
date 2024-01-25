[![codecov](https://codecov.io/gh/lazy-coders/lzc-coro/graph/badge.svg?token=O1N5INRR9G)](https://codecov.io/gh/lazy-coders/lzc-coro) [![CMake on Ubuntu with G++-13](https://github.com/lazy-coders/lzc-coro/actions/workflows/cmake-ubuntu-g++13.yml/badge.svg)](https://github.com/lazy-coders/lzc-coro/actions/workflows/cmake-ubuntu-g++13.yml)

# lzc-coro - LaZyCoders COROutine library

This small library have the objective of understand coroutine facility. It is
too a playground to test some ideas about how coroutines could work.

# The basics

This library only implements the `task`: a coroutine that can do `co_await`.

All `tasks` can be awaitable and are created stopped. That means a coroutine do
not start executing code until the call to `operator()` of the `task` is called.

```mermaid
sequenceDiagram
    box Grey Main Thread
        participant main
        participant some_task
        participant other_task
    end
    box Grey  Other Thread
        participant async_function
    end
    main->>some_task: operator()
    some_task->>other_task: co_await
    other_task->>async_function: co_await
    Note over other_task,async_function: The coroutine wait until completion<br/>and continues in the original thread
    async_function-->>other_task:resume()
    other_task-->>some_task:resume()
    some_task-->>main:future.get()
```
