#include "ButtonMcxAdapter.h"
#include <widgets/Button.h>
#include <mcx/McxEngine.h>

namespace mc::mcx {
    Shared<BaseWidget> ButtonMcxAdapter::createWidgetInstance(Shared<McxNode>& mcxNode) {
        return MakeRef<Button>();
    }

    void ButtonMcxAdapter::extractProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        auto button = std::static_pointer_cast<Button>(widget);

        mcxNode->setAttribute("backgroundColor", _toString(button->backgroundColor));
        mcxNode->setAttribute("borderColor", _toString(button->borderColor));
        mcxNode->setAttribute("cornerRadius", _toString(button->cornerRadius));
        mcxNode->setAttribute("borderThickness", _toString(button->borderThickness));
        mcxNode->setAttribute("leftText", button->secondaryLeftText);
        mcxNode->setAttribute("rightText", button->secondaryRightText);
        mcxNode->setAttribute("hoverOnColor", _toString(button->hoverOnColor));
        mcxNode->setAttribute("mousePressedColor", _toString(button->mousePressedColor));
        mcxNode->setAttribute("text", button->label->text);
        mcxNode->setAttribute("font", button->label->font);
        mcxNode->setAttribute("fontSize", _toString(button->label->fontSize));
        mcxNode->setAttribute("fontStyle", button->label->fontStyle);
        mcxNode->setAttribute("textAlignment", button->label->alignment);
        mcxNode->setAttribute("verticalPadding", _toString(button->label->verticalPadding));
        mcxNode->setAttribute("horizontalPadding", _toString(button->label->horizontalPadding));
        mcxNode->setAttribute("textColor", _toString(button->label->color));
        mcxNode->setAttribute("imagePlacement",
                (button->imagePlacement == ButtonImagePlacement::Cover) ? "cover" : "icon");
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

    std::vector<std::string> ButtonMcxAdapter::getAvailableProperties() {
        static const std::vector<std::string> props = {
            "backgroundColor", "borderColor", "cornerRadius",
            "borderThickness", "leftText", "rightText",
            "hoverOnColor", "mousePressedColor",
            "textColor", "text", "font", "fontSize", "fontStyle",
            "textAlignment", "verticalPadding", "horizontalPadding",
            "imagePlacement"
        };

        return props;
    }

    void ButtonMcxAdapter::_onCreateMcxNodeFromWidget(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& node
    ) {
        auto button = std::static_pointer_cast<Button>(widget);
        auto buttonImage = button->getImage();

        if (!buttonImage) {
            return;
        }

        // Get the image's path/url source
        auto imgSource = buttonImage->getOriginSrc();

        // Create the image node
        auto imageAdapter = McxEngine::getMcxAdapter("Image");
        auto imageNode = imageAdapter->createMcxNodeFromWidget(buttonImage);

        if (buttonImage->wasLoadedFromWeb()) {
            imageNode->setAttribute("url", imgSource);
        } else {
            imageNode->setAttribute("path", imgSource);
        }

        node->getChildren().push_back(imageNode);
    }
} //namespace mc::mcx
