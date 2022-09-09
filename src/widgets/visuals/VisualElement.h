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
    VisualTypeImage,
    VisualTypeClipRegion
};

class VisualElement {
public:
    virtual ~VisualElement() = default;
    virtual const VisualType type() const = 0;
    virtual Size measureSize() const = 0;

    VisualElement() = default;

    PropertyObserver<bool>        visible       = PropertyObserver<bool>(true);
    PropertyObserver<Position>    position      = PropertyObserver<Position>({ 0, 0 });
    PropertyObserver<Color>       color         = PropertyObserver<Color>(Color::black);
    PropertyObserver<uint32_t>    customWidth   = PropertyObserver<uint32_t>(0);
    PropertyObserver<uint32_t>    customHeight  = PropertyObserver<uint32_t>(0);
};
} // namespace mc
