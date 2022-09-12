#include "ButtonMcxAdapter.h"
#include <widgets/Button.h>

namespace mc::mcx {
    Shared<BaseWidget> ButtonMcxAdapter::createWidgetInstance() {
        return MakeRef<Button>();
    }

    void ButtonMcxAdapter::applyProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        auto button = std::static_pointer_cast<Button>(widget);
        button->backgroundColor =
            mcxNode->getColorAttribute("backgroundColor", button->backgroundColor);

        button->borderColor =
            mcxNode->getColorAttribute("borderColor", button->borderColor);

        button->cornerRadius =
            mcxNode->getUIntAttribute("cornerRadius", button->cornerRadius);

        button->borderThickness =
            mcxNode->getUIntAttribute("borderThickness", button->borderThickness);

        button->secondaryLeftText =
            mcxNode->getAttribute("leftText", button->secondaryLeftText);

        button->secondaryRightText =
            mcxNode->getAttribute("rightText", button->secondaryRightText);

        button->hoverOnColor =
            mcxNode->getColorAttribute("hoverOnColor", button->hoverOnColor);

        button->mousePressedColor =
            mcxNode->getColorAttribute("mousePressedColor", button->mousePressedColor);

        button->label->text =
            mcxNode->getAttribute("text", button->label->text);

        button->label->font =
            mcxNode->getAttribute("font", button->label->font);

        button->label->fontSize =
            mcxNode->getUIntAttribute("fontSize", button->label->fontSize);

        button->label->alignment =
            mcxNode->getAttribute("textAlignment", button->label->alignment);

        button->label->fontStyle =
            mcxNode->getAttribute("fontStyle", button->label->fontStyle);

        button->label->verticalPadding =
            mcxNode->getUIntAttribute("verticalPadding", button->label->verticalPadding);

        button->label->horizontalPadding =
            mcxNode->getUIntAttribute("horizontalPadding", button->label->horizontalPadding);
    }
} //namespace mc::mcx
