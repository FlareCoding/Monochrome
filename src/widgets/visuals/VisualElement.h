#pragma once
#include "../Layer.h"
#include "../Color.h"
#include <events/PropertyObserver.h>

namespace mc {
enum VisualType {
    VisualTypeRect,
    VisualTypeCircle,
    VisualTypeBorder,
    VisualTypeText,
    VisualTypeImage
};

class VisualElement {
public:
    virtual ~VisualElement() = default;
    virtual const VisualType type() const = 0;

    VisualElement() = default;

    PropertyObserver<bool>        visible   = PropertyObserver<bool>(true);
    PropertyObserver<Position>    position  = PropertyObserver<Position>({ 0, 0 });
    PropertyObserver<Size>        size      = PropertyObserver<Size>({ 0, 0 });
    PropertyObserver<Color>       color     = PropertyObserver<Color>(Color::black);
};
} // namespace mc
