#include "ButtonMcxAdapter.h"
#include <widgets/Button.h>
#include <mcx/McxEngine.h>

namespace mc::mcx {
    Shared<BaseWidget> ButtonMcxAdapter::createWidgetInstance(Shared<McxNode>& mcxNode) {
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

        button->label->color =
            mcxNode->getColorAttribute("textColor", button->label->color);

        auto imagePlacement = mcxNode->getAttribute("imagePlacement", "cover");
        if (imagePlacement == "cover") {
            button->imagePlacement = ButtonImagePlacement::Cover;
        } else if (imagePlacement == "icon") {
            button->imagePlacement = ButtonImagePlacement::Icon;
        }

        // Only raw buttons can have an image child node
        if (mcxNode->getType() != "Button") {
            return;
        }

        mcxNode->childrenHandled = true;

        auto& children = mcxNode->getChildren();
        if (children.empty()) {
            return;
        }

        if (children.size() > 1) {
            printf("Button can have only one child of type Image to set the button's image\n");
            return;
        }

        auto& childNode = children.at(0);
        if (childNode->getType() != "Image") {
            printf("Button can have only one child of type Image to set the button's image\n");
            return;
        }

        auto imageInstance = std::static_pointer_cast<Image>(McxEngine::parseWidget(childNode));
        button->setImage(imageInstance);
    }
} //namespace mc::mcx
