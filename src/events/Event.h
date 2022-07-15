#pragma once
#include <core/Core.h>
#include <string>
#include <map>
#include <any>

namespace mc {
using eventDataMap_t = std::map<std::string, std::any>;

class Event {
public:
    template <typename T>
    T get(const std::string& property) {
        CORE_ASSERT(
            (d_data.find(property) != d_data.end()),
            "Event '" + name + "' doesn't have property '" + property + "'"
        );

        return std::any_cast<T>(d_data.at(property));
    }

    template <typename T>
    void store(const std::string& property, T value) {
        d_data[property] = value;
    }

    inline const bool isHandled() const { return !d_shouldPropagate; }
    inline void stopPropagation() { d_shouldPropagate = false; }

    std::string name;

    Event() = default;
    explicit Event(const eventDataMap_t& data) : d_data(data) {}

    static Shared<Event> empty;

protected:
    eventDataMap_t d_data;
    bool d_shouldPropagate = true;
};
} // namespace mc
