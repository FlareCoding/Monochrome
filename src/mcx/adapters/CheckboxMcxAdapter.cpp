#include "CheckboxMcxAdapter.h"
#include <widgets/Checkbox.h>

namespace mc::mcx {
    Shared<BaseWidget> CheckboxMcxAdapter::createWidgetInstance() {
        return MakeRef<Checkbox>();
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
} //namespace mc::mcx
