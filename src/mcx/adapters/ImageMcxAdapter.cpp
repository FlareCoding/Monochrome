#include "ImageMcxAdapter.h"
#include <widgets/Image.h>

namespace mc::mcx {
    Shared<BaseWidget> ImageMcxAdapter::createWidgetInstance(Shared<McxNode>& mcxNode) {
        auto webUrl = mcxNode->getAttribute("url");
        if (!webUrl.empty()) {
            return Image::loadFromWebUrl(webUrl);
        }

        auto localPath = mcxNode->getAttribute("path");
        if (!localPath.empty()) {
            return Image::loadFromFile(localPath);
        }

        return MakeRef<Image>(nullptr);
    }

    void ImageMcxAdapter::applyProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        auto image = std::static_pointer_cast<Image>(widget);
        image->opacity =
            mcxNode->getUIntAttribute("opacity", image->opacity);
    }
} //namespace mc::mcx
