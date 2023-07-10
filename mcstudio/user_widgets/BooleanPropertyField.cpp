#include "BooleanPropertyField.h"

namespace mc::mcstudio {
    BooleanPropertyField::BooleanPropertyField(
        const std::string& name,
        const std::string& value
    ) : StackPanel() {
        d_propName = name;
        d_propValue = value;

        appendAllowedEvent("widgetPropertyModified");

        _init();
    }

    void BooleanPropertyField::_init() {
        bool propBoolVal = (d_propValue == "true");

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

        auto switchStackPanel = MakeRef<StackPanel>();
        switchStackPanel->orientation = Horizontal;
        switchStackPanel->backgroundColor = Color::transparent;
        switchStackPanel->fixedHeight = 20;

        d_falseButton = MakeRef<Button>();
        d_falseButton->fixedHeight = 16;
        d_falseButton->verticalAlignment = VACenter;
        d_falseButton->label->text = "false";
        d_falseButton->label->color = Color::white;
        d_falseButton->backgroundColor = Color(100, 0, 0);
        d_falseButton->borderColor = Color::transparent;
        d_falseButton->cornerRadius = 0;
        d_falseButton->on("clicked", &BooleanPropertyField::_propChangedToFalse, this);
        switchStackPanel->addChildOffline(d_falseButton);

        d_trueButton = MakeRef<Button>();
        d_trueButton->fixedHeight = 16;
        d_trueButton->verticalAlignment = VACenter;
        d_trueButton->label->text = "true";
        d_trueButton->label->color = Color::white;
        d_trueButton->backgroundColor = Color(0, 100, 0);
        d_trueButton->borderColor = Color::transparent;
        d_trueButton->cornerRadius = 0;
        d_trueButton->on("clicked", &BooleanPropertyField::_propChangedToTrue, this);
        switchStackPanel->addChildOffline(d_trueButton);

        if (propBoolVal) {
            d_falseButton->backgroundColor = d_dimmedColor;
        } else {
            d_trueButton->backgroundColor = d_dimmedColor;
        }

        addChildOffline(switchStackPanel);
    }

    void BooleanPropertyField::_propChangedToTrue(Shared<Event> e) {
        d_propValue = "true";

        // Highlight the "true" button
        d_trueButton->backgroundColor = Color(0, 100, 0);

        // Dim the "false" button
        d_falseButton->backgroundColor = d_dimmedColor;

        fireEvent("widgetPropertyModified", {
            { "propName", d_propName },
            { "propValue",d_propValue },
        });
    }

    void BooleanPropertyField::_propChangedToFalse(Shared<Event> e) {
        d_propValue = "false";

        // Highlight the "false" button
        d_falseButton->backgroundColor = Color(100, 0, 0);

        // Dim the "true" button
        d_trueButton->backgroundColor = d_dimmedColor;

        fireEvent("widgetPropertyModified", {
            { "propName", d_propName },
            { "propValue", d_propValue },
        });
    }
} // namespace mc::mcstudio
