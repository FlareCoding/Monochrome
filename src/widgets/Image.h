#pragma once
#include "BaseWidget.h"
#include "visuals/ImageVisual.h"

namespace mc {
class Image : public BaseWidget {
public:
    static Shared<Image> loadFromFile(const std::string& path);
    static Shared<Image> loadFromWebUrl(const std::string& url);

    explicit Image(Shared<Bitmap> bitmap, const std::string& source = "");

    std::string getWidgetName() const override { return "Image"; }

    // 0-255 value determining the transparency level of the image
    PropertyObserver<uint32_t> opacity;

    // Specifies if the image should get tiled
    PropertyObserver<bool> tilingEnabled;

    inline bool wasLoadedFromWeb() const { return d_loadedFromWeb; }
    inline std::string getOriginSrc() const { return d_originSource; }

    inline Shared<Bitmap> getBitmap() const { return d_bitmap; }

protected:
    Size _measureSize() override;

private:
    void _createVisuals();
    void _setupProperties();

    Shared<Bitmap>      d_bitmap;
    Shared<ImageVisual> d_imageVisual;

    bool d_loadedFromWeb = false;
    std::string d_originSource;
};
} // namespace mc
