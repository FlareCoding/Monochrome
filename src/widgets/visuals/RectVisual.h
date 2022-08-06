#pragma once
#include "VisualElement.h"

namespace mc {
class RectVisual : public VisualElement {
public:
    inline const VisualType type() const override { return VisualTypeRect; }

    PropertyObserver<uint32_t> cornerRadius;
};
} // namespace mc
