#include "LabelMcxAdapter.h"
#include <widgets/Label.h>

namespace mc::mcx {
    Shared<BaseWidget> LabelMcxAdapter::createWidgetInstance(Shared<McxNode>& mcxNode) {
        return MakeRef<Label>();
    }

    void LabelMcxAdapter::applyProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        auto label = std::static_pointer_cast<Label>(widget);
        label->color =
            mcxNode->getColorAttribute("color", label->color);

        label->text =
            mcxNode->getAttribute("text", label->text);

        label->font =
            mcxNode->getAttribute("font", label->font);

        label->fontSize =
            mcxNode->getUIntAttribute("fontSize", label->fontSize);

        label->alignment =
            mcxNode->getAttribute("textAlignment", label->alignment);

        label->fontStyle =
            mcxNode->getAttribute("fontStyle", label->fontStyle);

        label->verticalPadding =
            mcxNode->getUIntAttribute("verticalPadding", label->verticalPadding);

        label->horizontalPadding =
            mcxNode->getUIntAttribute("horizontalPadding", label->horizontalPadding);
    }

    std::vector<std::string> LabelMcxAdapter::getAvailableProperties() {
        static const std::vector<std::string> props = {
            "color", "text", "font", "fontSize",
            "textAlignment", "fontStyle",
            "verticalPadding", "horizontalPadding"
        };

        return props;
    }
} //namespace mc::mcx
