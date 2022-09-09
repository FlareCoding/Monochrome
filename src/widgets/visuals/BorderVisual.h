#pragma once
#include "VisualElement.h"

namespace mc {
class BorderVisual : public VisualElement {
public:
    inline const VisualType type() const override { return VisualTypeBorder; }
    inline Size measureSize() const override { return Size(customWidth, customHeight); }

    PropertyObserver<uint32_t> cornerRadius;
    PropertyObserver<uint32_t> thickness;
};
} // namespace mc
