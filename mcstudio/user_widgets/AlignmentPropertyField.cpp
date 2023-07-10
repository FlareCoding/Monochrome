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

        auto leftAlignButton = _createAlignmentImgButton("left", 1);
        addChildOffline(leftAlignButton);

        auto centerAlignButton = _createAlignmentImgButton("center", 2);
        addChildOffline(centerAlignButton);

        auto rightAlignButton = _createAlignmentImgButton("right", 3);
        addChildOffline(rightAlignButton);

        auto fillAlignButton = _createAlignmentImgButton("fill", 4);
        addChildOffline(fillAlignButton);
    }

    Shared<Button> AlignmentPropertyField::_createAlignmentImgButton(
        const std::string& alignment,
        uint32_t orderIdx
    ) {
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
        button->setImage(Image::loadFromFile("icons/align-" + alignment + ".png"));
        button->on("clicked", &AlignmentPropertyField::_alignmentControlButton_OnClick, this);

        return button;
    }

    void AlignmentPropertyField::_alignmentControlButton_OnClick(Shared<Event> e) {
        auto orderIdx = e->target->zIndex.get();

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
} // namespace mc::mcstudio
