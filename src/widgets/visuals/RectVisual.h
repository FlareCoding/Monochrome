#pragma once
#include "VisualElement.h"

namespace mc {
class RectVisual : public VisualElement {
public:
    inline const VisualType type() const override { return VisualTypeRect; }

    uint32_t cornerRadius = 2;
};
} // namespace mc
