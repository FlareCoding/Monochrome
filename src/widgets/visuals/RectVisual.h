#pragma once
#include "VisualElement.h"

namespace mc {
class RectVisual : public VisualElement {
public:
    inline const VisualType type() const override { return VisualTypeRect; }
    inline Size measureSize() const override { return Size(customWidth, customHeight); }

    PropertyObserver<uint32_t> cornerRadius;
};
} // namespace mc
