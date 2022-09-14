#pragma once
#include "VisualElement.h"
#include <rendering/Bitmap.h>

namespace mc {
class ImageVisual : public VisualElement {
public:
    inline const VisualType type() const override { return VisualTypeImage; }
    inline Size measureSize() const override {
        if (imageBitmap) {
            return Size(imageBitmap->getWidth(), imageBitmap->getHeight());
        }

        return Size(customWidth, customHeight);
    }

    Shared<Bitmap> imageBitmap;
    PropertyObserver<uint32_t> opacity = PropertyObserver<uint32_t>(255);
};
} // namespace mc
