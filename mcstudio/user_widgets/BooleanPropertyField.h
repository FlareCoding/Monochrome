#pragma once
#include <widgets/AllWidgets.h>

namespace mc::mcstudio {
class BooleanPropertyField : public StackPanel {
public:
    BooleanPropertyField(
        const std::string& name,
        const std::string& value
    );

private:
    Shared<Button> d_trueButton;
    Shared<Button> d_falseButton;

    const Color d_dimmedColor = Color(53, 54, 59);

private:
    std::string d_propName;
    std::string d_propValue;

    void _init();

    void _propChangedToTrue(Shared<Event> e);
    void _propChangedToFalse(Shared<Event> e);
};
} // namespace mc::mcstudio
