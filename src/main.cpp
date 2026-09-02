#include "DeviceEvent.h"
#include "ThreadPool.h"

#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

namespace {

std::mutex coutMutex;

std::string eventTypeToString(DeviceEventType type)
{
    switch (type) {
        case DeviceEventType::ButtonPress:
            return "ButtonPress";
        case DeviceEventType::TemperatureUpdate:
            return "TemperatureUpdate";
        case DeviceEventType::NetworkPacket:
            return "NetworkPacket";
        case DeviceEventType::SensorUpdate:
            return "SensorUpdate";
    }

    return "Unknown";
}

void processDeviceEvent(const DeviceEvent& event)
{
    // Simulate CPU/I/O work performed by an application worker.
    std::this_thread::sleep_for(1000ms + std::chrono::milliseconds(event.value % 1000));

    std::lock_guard<std::mutex> lock(coutMutex);

    std::cout
        << "[worker " << std::this_thread::get_id() << "] "
        << "processed event #" << event.id
        << " | device=" << event.deviceName
        << " | type=" << eventTypeToString(event.type)
        << " | value=" << event.value
        << '\n';
}

} // namespace

int main()
{
    constexpr std::size_t workerCount = 4;
    constexpr std::size_t eventCount = 12;

    ThreadPool pool(workerCount);

    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Thread pool started with "
                  << pool.size() << " workers.\n\n";
    }

    std::vector<std::future<void>> futures;
    futures.reserve(eventCount);

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> valueDistribution(1, 100);

    const std::vector<DeviceEventType> eventTypes = {
        DeviceEventType::ButtonPress,
        DeviceEventType::TemperatureUpdate,
        DeviceEventType::NetworkPacket,
        DeviceEventType::SensorUpdate
    };

    // Simulate a device event producer.
    for (std::uint64_t id = 1; id <= eventCount; ++id) {
        DeviceEvent event{
            id,
            eventTypes[(id - 1) % eventTypes.size()],
            valueDistribution(rng),
            "device-" + std::to_string((id % 3) + 1)
        };

        futures.push_back(
            pool.submit(processDeviceEvent, std::move(event))
        );

        std::this_thread::sleep_for(300ms);
    }

    // Wait for all submitted events to complete.
    for (auto& future : futures) {
        future.get();
    }

    std::cout << "\nAll device events processed successfully.\n";

    return 0;
}
