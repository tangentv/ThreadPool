#include "ThreadPool.h"

#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <vector>

int main()
{
    ThreadPool pool(4);

    // Verify returned futures.
    auto future = pool.submit([](int a, int b) {
        return a + b;
    }, 10, 20);

    assert(future.get() == 30);

    // Verify multiple tasks execute correctly.
    std::atomic<int> counter{0};
    std::vector<std::future<void>> futures;

    for (int i = 0; i < 100; ++i) {
        futures.push_back(pool.submit([&counter] {
            counter.fetch_add(1, std::memory_order_relaxed);
        }));
    }

    for (auto& f : futures) {
        f.get();
    }

    assert(counter.load() == 100);

    // Verify exceptions are propagated through future.
    auto failingTask = pool.submit([]() -> int {
        throw std::runtime_error("test exception");
    });

    bool exceptionCaught = false;

    try {
        (void)failingTask.get();
    } catch (const std::runtime_error&) {
        exceptionCaught = true;
    }

    assert(exceptionCaught);

    std::cout << "All thread pool tests passed.\n";
    return 0;
}
