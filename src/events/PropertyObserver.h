#pragma once
#include <events/EventEmitter.h>

namespace mc {
template <typename T>
class PropertyObserver : public EventEmitter {
    T _property;

public:
    PropertyObserver() : _property(T()), EventEmitter({"propertyChanged"}) {}
    explicit PropertyObserver(const T& val) : _property(val), EventEmitter({"propertyChanged"}) {}

    operator T() const {
        return _property;
    }

    T* operator->() const {
        return const_cast<T*>(&_property);
    }

    auto& operator=(const T& rvalue) {
        _property = rvalue;

        fireEvent("propertyChanged", Event::empty);
        return *this;
    }

    auto& operator=(const PropertyObserver<T>& rvalue) {
        _property = rvalue.get();

        fireEvent("propertyChanged", Event::empty);
        return *this;
    }

    const T& get() const {
        return _property;
    }
};
} // namespace mc
