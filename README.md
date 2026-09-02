# C++ Thread Pool Application

A small, production-style C++17 thread pool demonstrating:

- A fixed set of worker threads
- A thread-safe task queue
- `std::condition_variable` for efficient worker sleeping/wakeup
- Generic task submission with `std::future`
- Graceful shutdown
- A practical device-event processing example
- Clean separation between the thread pool and application logic

## Project structure

```text
thread_pool_application/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── ThreadPool.h
│   └── DeviceEvent.h
├── src/
│   ├── ThreadPool.cpp
│   └── main.cpp
└── tests/
    └── thread_pool_test.cpp
```

## Build

Requirements:
- C++17 compiler
- CMake 3.16+

```bash
mkdir -p build
cd build
cmake ..
cmake --build . -j
```

## Run

From the project root:

```bash
./build/thread_pool_app
```

## Run tests

```bash
cd build
ctest --output-on-failure
```

## What to study

Start with `include/ThreadPool.h` and `src/ThreadPool.cpp`.

The important flow is:

```text
submit(task)
   |
   v
task queue <---- mutex
   |
   v
condition_variable
   |
   v
worker thread
   |
   v
execute task
   |
   v
std::future / result
```

The example application simulates device events arriving from a producer and being processed concurrently by the thread pool.
