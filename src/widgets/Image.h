#pragma once
#include "BaseWidget.h"
#include "visuals/ImageVisual.h"

namespace mc {
class Image : public BaseWidget {
public:
    static Shared<Image> loadFromFile(const std::string& path);
    static Shared<Image> loadFromWebUrl(const std::string& url);

    explicit Image(Shared<Bitmap> bitmap);

protected:
    Size _measureSize() override;

private:
    void _createVisuals();

    Shared<ImageVisual> d_imageVisual;
};
} // namespace mc
