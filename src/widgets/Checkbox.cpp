#include "Button.h"
#include "Checkbox.h"

namespace mc {
    Checkbox::Checkbox() {
        appendAllowedEvent("valueChanged");

        _setupProperties();
    }

    Checkbox::Checkbox(const std::string& text) : Checkbox() {
        label->text = text;
    }

    Size Checkbox::_measureSize() {
        // Get the label's desired size
        auto labelDesiredSize = label->getDesiredSize();

        auto buttonWidth = static_cast<uint32_t>(
            static_cast<float>(labelDesiredSize.height * 0.75f)
        );

        Size checkboxSize = Size(0, 0);

        // Set the checkbox's width to be the sum of the sizes
        // of the button and the label plus the margins in between.
        checkboxSize.width =
            buttonWidth + labelDesiredSize.width + button->marginRight;

        // Set the checkbox's to fit both the button and the label
        checkboxSize.height = labelDesiredSize.height;

        return checkboxSize;
    }

    void Checkbox::_onArrangeChildren() {
        // Get the size of the frame allocated for the checkbox
        auto checkboxBounds = getComputedSize();

        // Make the button's final size slightly smaller
        // than the entire height of the checkbox's frame.
        auto buttonDesiredSize = button->getDesiredSize();
        buttonDesiredSize.height = static_cast<uint32_t>(
            static_cast<float>(checkboxBounds.height * 0.75f)
        );

        // Make sure the button is a square
        buttonDesiredSize.width = buttonDesiredSize.height;

        // Finalize button's size and position
        button->setComputedSize(buttonDesiredSize);
        button->position = Position(0, static_cast<uint32_t>(
            static_cast<float>(checkboxBounds.height * 0.175f)
        ));

        // Set the height of the label's final size
        // to be the same as the checkbox's bounding height.
        auto labelSize = label->getDesiredSize();
        labelSize.height = checkboxBounds.height;

        // Finalize the label's size and position
        label->setComputedSize(labelSize);
        label->position->x = button->getComputedSize().width + button->marginRight;
    }

    void Checkbox::_setupProperties() {
        button = MakeRef<Button>();
        button->label->text = d_checkmarkUtf8Prefix;
        button->label->fontSize = 10;
        button->label->visible = false;
        button->marginRight = 2;
        cursorType.forwardAssignment(&button->cursorType);
        _addChildOffline(button);

        label = MakeRef<Label>();
        label->text = "Checkbox";
        label->horizontalPadding = 16;
        cursorType.forwardAssignment(&label->cursorType);
        _addChildOffline(label);

        cursorType = CursorType::Hand;

        checked = false;
        checked.on("propertyChanged", [this](Shared<Event> e) {
            button->label->visible = checked.get();

            fireEvent("valueChanged", {
                { "checked", checked.get() }
            });
        });
        handleWidgetVisiblePropertyChange(checked);

        enabled = true;
        handleWidgetVisiblePropertyChange(enabled);
        enabled.on("propertyChanged", &Checkbox::_toggleEnabledState, this);

        on("clicked", &Checkbox::_onClick, this);
    }

    void Checkbox::_onClick(Shared<Event> e) {
        checked = !checked;
        button->label->visible = checked;

        fireEvent("valueChanged", {
            { "checked", checked.get() }
        });
    }

    void Checkbox::_toggleEnabledState(Shared<Event> e) {
        if (d_isEnabled == enabled.get()) {
            return;
        }

        d_isEnabled = enabled.get();
        button->enabled = d_isEnabled;

        if (d_isEnabled) {
            enablePointerEvents();
            label->enablePointerEvents();

            auto labelColor = label->color;
            labelColor->undim(4);
            label->color = labelColor;
        }
        else {
            disablePointerEvents();
            label->disablePointerEvents();

            auto labelColor = label->color;
            labelColor->dim(4);
            label->color = labelColor;
        }
    }
} // namespace mc
