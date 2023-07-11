#include "AlignmentPropertyField.h"

namespace mc::mcstudio {
    AlignmentPropertyField::AlignmentPropertyField(
        const std::string& name,
        const std::string& value
    ) : StackPanel() {
        d_propName = name;
        d_propValue = value;

        appendAllowedEvent("widgetPropertyModified");

        _init();
    }

    void AlignmentPropertyField::_init() {
        this->orientation = Horizontal;
        this->backgroundColor = Color::transparent;
        this->marginBottom = 4;

        auto propertyNameLabel = MakeRef<Label>();
        propertyNameLabel->text = d_propName;
        propertyNameLabel->fixedWidth = 110;
        propertyNameLabel->fixedHeight = 30;
        propertyNameLabel->marginLeft = 10;
        propertyNameLabel->marginRight = 4;
        propertyNameLabel->alignment = "left";
        addChildOffline(propertyNameLabel);

        Shared<Button>  alignControlButton1,
                        alignControlButton2,
                        alignControlButton3,
                        alignControlButton4;

        if (d_propName == "horizontalAlignment") {
            alignControlButton1 = _createAlignmentImgButton("left", 1);
            alignControlButton2 = _createAlignmentImgButton("center", 2);
            alignControlButton3 = _createAlignmentImgButton("right", 3);
            alignControlButton4 = _createAlignmentImgButton("fill", 4);

        } else if (d_propName == "verticalAlignment") {
            alignControlButton1 = _createAlignmentImgButton("top", 1);
            alignControlButton2 = _createAlignmentImgButton("center", 2);
            alignControlButton3 = _createAlignmentImgButton("bottom", 3);
            alignControlButton4 = _createAlignmentImgButton("fill", 4);
        }

        addChildOffline(alignControlButton1);
        addChildOffline(alignControlButton2);
        addChildOffline(alignControlButton3);
        addChildOffline(alignControlButton4);

        if (d_propValue == "left" || d_propValue == "top") {
            alignControlButton1->backgroundColor = d_selectedControlButtonColor;

        } else if (d_propValue == "center") {
            alignControlButton2->backgroundColor = d_selectedControlButtonColor;

        } else if (d_propValue == "right" || d_propValue == "bottom") {
            alignControlButton3->backgroundColor = d_selectedControlButtonColor;
        } else {
            alignControlButton4->backgroundColor = d_selectedControlButtonColor;
        }
    }

    Shared<Button> AlignmentPropertyField::_createAlignmentImgButton(
        const std::string& alignment,
        uint32_t orderIdx
    ) {
        std::string imgPath = "";
        if (d_propName == "horizontalAlignment") {
            imgPath = "icons/halign-" + alignment + ".png";
        } else if (d_propName == "verticalAlignment") {
            imgPath = "icons/valign-" + alignment + ".png";
        }

        auto button = MakeRef<Button>();
        button->zIndex = orderIdx;
        button->borderColor = Color::transparent;
        button->backgroundColor = getCustomColor("mcstudioBgColorPrimary");
        button->marginLeft = 4;
        button->marginRight = 4;
        button->label->text = "";
        button->fixedWidth = 24;
        button->fixedHeight = 24;
        button->imagePlacement = Cover;
        button->setImage(Image::loadFromFile(imgPath));
        button->on("clicked", &AlignmentPropertyField::_alignmentControlButton_OnClick, this);

        return button;
    }

    void AlignmentPropertyField::_alignmentControlButton_OnClick(Shared<Event> e) {
        _dimAllControlButtons();

        auto orderIdx = e->target->zIndex.get();

        auto targetBtn = std::static_pointer_cast<Button>(getChild(orderIdx));
        targetBtn->backgroundColor = d_selectedControlButtonColor;

        if (d_propName == "horizontalAlignment") {
            switch (orderIdx) {
            case 1: {
                d_propValue = "left";
                break;
            }
            case 2: {
                d_propValue = "center";
                break;
            }
            case 3: {
                d_propValue = "right";
                break;
            }
            case 4: {
                d_propValue = "fill";
                break;
            }
            default: break;
            }
        } else if (d_propName == "verticalAlignment") {
            switch (orderIdx) {
            case 1: {
                d_propValue = "top";
                break;
            }
            case 2: {
                d_propValue = "center";
                break;
            }
            case 3: {
                d_propValue = "bottom";
                break;
            }
            case 4: {
                d_propValue = "fill";
                break;
            }
            default: break;
            }
        }

        fireEvent("widgetPropertyModified", {
            { "propName", d_propName },
            { "propValue", d_propValue },
        });
    }

    void AlignmentPropertyField::_dimAllControlButtons() {
        for (uint64_t i = 1; i < 5; ++i) {
            auto btn = std::static_pointer_cast<Button>(getChild(i));
            btn->backgroundColor = getCustomColor("mcstudioBgColorPrimary");
        }
    }
} // namespace mc::mcstudio
