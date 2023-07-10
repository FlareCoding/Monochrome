#pragma once
#include <widgets/AllWidgets.h>

namespace mc::mcstudio {
class RegularPropertyTextField : public StackPanel {
public:
    RegularPropertyTextField(
        const std::string& name,
        const std::string& value
    );

private:
    std::string d_propName;
    std::string d_propValue;

    void _init();

    void _onValueEntryChanged(Shared<Event> e);
};
} // namespace mc::mcstudio
