#include "RegularPropertyTextField.h"

namespace mc::mcstudio {
    RegularPropertyTextField::RegularPropertyTextField(
        const std::string& name,
        const std::string& value
    ) : StackPanel() {
        d_propName = name;
        d_propValue = value;

        appendAllowedEvent("widgetPropertyModified");

        _init();
    }

    void RegularPropertyTextField::_init() {
        this->orientation = Horizontal;
        this->backgroundColor = Color::transparent;
        this->marginBottom = 4;

        auto propertyNameLabel = MakeRef<Label>();
        propertyNameLabel->text = d_propName;
        propertyNameLabel->fixedWidth = 110;
        propertyNameLabel->fixedHeight = 20;
        propertyNameLabel->marginLeft = 10;
        propertyNameLabel->alignment = "left";
        addChildOffline(propertyNameLabel);

        auto entry = MakeRef<Entry>();
        entry->backgroundColor = Color::transparent;
        entry->borderColor = Color::transparent;
        entry->textColor = Color::white;
        entry->cursorColor = Color::white;
        entry->focusedBorderColor = Color(0, 240, 216);
        entry->text = d_propValue;
        entry->fixedWidth = 150;
        entry->fixedHeight = 20;
        entry->on("entered", &RegularPropertyTextField::_onValueEntryChanged, this);
        addChildOffline(entry);
    }

    void RegularPropertyTextField::_onValueEntryChanged(Shared<Event> e) {
        auto target = static_cast<Entry*>(e->target);
        auto newValue = target->text.get();

        d_propValue = newValue;

        fireEvent("widgetPropertyModified", {
            { "propName", d_propName },
            { "propValue", newValue },
        });
    }
} // namespace mc::mcstudio
