#pragma once
#include "VisualElement.h"

namespace mc {
class ClipRegionVisual : public VisualElement {
public:
    inline const VisualType type() const override { return VisualTypeClipRegion; }
    inline Size measureSize() const override { return Size(customWidth, customHeight); }
};
} // namespace mc
