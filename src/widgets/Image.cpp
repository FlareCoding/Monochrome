#include "Image.h"

namespace mc {
    Shared<Image> Image::loadFromFile(const std::string& path) {
        auto bitmap = Bitmap::loadFromFile(path);

        return MakeRef<Image>(bitmap, path);
    }

    Shared<Image> Image::loadFromWebUrl(const std::string& url) {
        auto bitmap = Bitmap::loadFromWebUrl(url);

        auto img = MakeRef<Image>(bitmap, url);
        img->d_loadedFromWeb = true;

        return img;
    }

    Image::Image(Shared<Bitmap> bitmap, const std::string& source) {
        d_bitmap = bitmap;
        d_originSource = source;

        _createVisuals();
        _setupProperties();

        d_imageVisual->imageBitmap = bitmap;
        opacity.forwardAssignment(&d_imageVisual->opacity);
        tilingEnabled.forwardAssignment(&d_imageVisual->tilingEnabled);
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
        handleWidgetVisiblePropertyChange(opacity);

        tilingEnabled = false;
        handleWidgetVisiblePropertyChange(tilingEnabled);
    }
} // namespace mc
