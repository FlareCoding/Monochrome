#pragma once
#include "BaseWidget.h"
#include "visuals/ImageVisual.h"

namespace mc {
class Image : public BaseWidget {
public:
    static Shared<Image> loadFromFile(const std::string& path);
    static Shared<Image> loadFromWebUrl(const std::string& url);

    explicit Image(Shared<Bitmap> bitmap);

    // 0-255 value determining the transparency level of the image
    PropertyObserver<uint32_t> opacity;

protected:
    Size _measureSize() override;

private:
    void _createVisuals();
    void _setupProperties();

    Shared<ImageVisual> d_imageVisual;
};
} // namespace mc
