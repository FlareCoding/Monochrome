#pragma once
#include <events/EventEmitter.h>

namespace mc {
template <typename T>
class PropertyObserver : public EventEmitter {
    T _property;
    std::vector<PropertyObserver<T>*> _dependants;

public:
    PropertyObserver() : _property(T()), EventEmitter({"propertyChanged"}) {}
    explicit PropertyObserver(const T& val) : _property(val), EventEmitter({"propertyChanged"}) {}

    operator T() const {
        return _property;
    }

    bool operator==(const PropertyObserver<T>& other) const {
        return this->_property == other.get();
    }

    bool operator!=(const PropertyObserver<T>& other) const {
        return this->_property != other.get();
    }

    T* operator->() const {
        return const_cast<T*>(&_property);
    }

    auto& operator=(const T& other) {
        set(other);

        fireEvent("propertyChanged", Event::empty);
        return *this;
    }

    auto& operator=(const PropertyObserver<T>& other) {
        set(other);

        fireEvent("propertyChanged", Event::empty);
        return *this;
    }

    const T& get() const {
        return _property;
    }

    inline void set(const T& val) {
        _property = val;

        for (auto& dependant : _dependants) {
            dependant->set(val);
        }
    }

    inline void set(const PropertyObserver<T>& other) {
        _property = other.get();

        for (auto& dependant : _dependants) {
            dependant->set(other.get());
        }
    }

    inline void forwardAssignment(PropertyObserver<T>* other) {
        _dependants.push_back(other);
    }
};
} // namespace mc
