#include "CheckboxMcxAdapter.h"
#include <widgets/Checkbox.h>

namespace mc::mcx {
    Shared<BaseWidget> CheckboxMcxAdapter::createWidgetInstance(Shared<McxNode>& mcxNode) {
        return MakeRef<Checkbox>();
    }

    void CheckboxMcxAdapter::extractProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        auto checkbox = std::static_pointer_cast<Checkbox>(widget);

        mcxNode->setAttribute("checked", _toString(checkbox->checked));
        mcxNode->setAttribute("backgroundColor", _toString(checkbox->button->backgroundColor));
        mcxNode->setAttribute("borderColor", _toString(checkbox->button->borderColor));
        mcxNode->setAttribute("cornerRadius", _toString(checkbox->button->cornerRadius));
        mcxNode->setAttribute("borderThickness", _toString(checkbox->button->borderThickness));
        mcxNode->setAttribute("text", checkbox->label->text);
        mcxNode->setAttribute("font", checkbox->label->font);
        mcxNode->setAttribute("fontSize", _toString(checkbox->label->fontSize));
        mcxNode->setAttribute("fontStyle", checkbox->label->fontStyle);
        mcxNode->setAttribute("textAlignment", checkbox->label->alignment);
        mcxNode->setAttribute("verticalPadding", _toString(checkbox->label->verticalPadding));
        mcxNode->setAttribute("horizontalPadding", _toString(checkbox->label->horizontalPadding));
        mcxNode->setAttribute("textColor", _toString(checkbox->label->color));
    }

    void CheckboxMcxAdapter::applyProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        auto checkbox = std::static_pointer_cast<Checkbox>(widget);
        checkbox->checked =
            mcxNode->getBoolAttribute("checked", checkbox->checked);

        checkbox->button->backgroundColor =
            mcxNode->getColorAttribute("backgroundColor", checkbox->button->backgroundColor);

        checkbox->button->borderColor =
            mcxNode->getColorAttribute("borderColor", checkbox->button->borderColor);

        checkbox->button->cornerRadius =
            mcxNode->getUIntAttribute("cornerRadius", checkbox->button->cornerRadius);

        checkbox->button->borderThickness =
            mcxNode->getUIntAttribute("borderThickness", checkbox->button->borderThickness);

        checkbox->label->color =
            mcxNode->getColorAttribute("textColor", checkbox->label->color);

        checkbox->label->text =
            mcxNode->getAttribute("text", checkbox->label->text);

        checkbox->label->font =
            mcxNode->getAttribute("font", checkbox->label->font);

        checkbox->label->fontSize =
            mcxNode->getUIntAttribute("fontSize", checkbox->label->fontSize);

        checkbox->label->alignment =
            mcxNode->getAttribute("textAlignment", checkbox->label->alignment);

        checkbox->label->fontStyle =
            mcxNode->getAttribute("fontStyle", checkbox->label->fontStyle);

        checkbox->label->verticalPadding =
            mcxNode->getUIntAttribute("verticalPadding", checkbox->label->verticalPadding);

        checkbox->label->horizontalPadding =
            mcxNode->getUIntAttribute("horizontalPadding", checkbox->label->horizontalPadding);
    }

    std::vector<std::string> CheckboxMcxAdapter::getAvailableProperties() {
        static const std::vector<std::string> props = {
            "checked", "backgroundColor", "borderColor",
            "cornerRadius", "borderThickness", "textColor",
            "text", "font", "fontSize", "fontStyle",
            "textAlignment", "verticalPadding", "horizontalPadding"
        };

        return props;
    }
} //namespace mc::mcx
