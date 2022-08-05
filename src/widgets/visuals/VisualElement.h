#pragma once
#include "../Layer.h"
#include "../Color.h"

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

    Position    position;
    Size        size;
    Color       color;
};
} // namespace mc
