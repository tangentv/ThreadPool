#pragma once

#include <cstdint>
#include <string>

enum class DeviceEventType {
    ButtonPress,
    TemperatureUpdate,
    NetworkPacket,
    SensorUpdate
};

struct DeviceEvent {
    std::uint64_t id{};
    DeviceEventType type{};
    int value{};
    std::string deviceName;
};
