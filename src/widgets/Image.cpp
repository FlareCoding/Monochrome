#include "Image.h"

namespace mc {
    Shared<Image> Image::loadFromFile(const std::string& path) {
        auto bitmap = Bitmap::loadFromFile(path);
        return MakeRef<Image>(bitmap);
    }

    Shared<Image> Image::loadFromWebUrl(const std::string& url) {
        auto bitmap = Bitmap::loadFromWebUrl(url);
        return MakeRef<Image>(bitmap);
    }

    Image::Image(Shared<Bitmap> bitmap) {
        _createVisuals();

        d_imageVisual->imageBitmap = bitmap;
        opacity.forwardAssignment(&d_imageVisual->opacity);
    }

    Size Image::_measureSize() {
        return d_imageVisual->measureSize();
    }

    void Image::_createVisuals() {
        d_imageVisual = MakeRef<ImageVisual>();
        addCoreVisualElement(d_imageVisual);
    }

    void Image::_setupProperties() {
        opacity = 255;
        opacity.forwardEmittedEvents(this);
    }
} // namespace mc
