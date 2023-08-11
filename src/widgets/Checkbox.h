#pragma once
#include "Label.h"
#include "Button.h"

namespace mc {
class Checkbox : public BaseWidget {
public:
    Checkbox();
    explicit Checkbox(const std::string& text);

    std::string getWidgetName() const override { return "Checkbox"; }

    Shared<Button> button;
    Shared<Label> label;

    PropertyObserver<bool> checked;

    // Specifies whether or not the checkbox is enabled and can be interacted with
    PropertyObserver<bool> enabled;

protected:
    Size _measureSize() override;
    void _onArrangeChildren() override;

private:
    void _setupProperties();
    void _onClick(Shared<Event> e);
    void _toggleEnabledState(Shared<Event> e);

    bool d_isEnabled = true;

    const char d_checkmarkUtf8Prefix[6] = {
        static_cast<char>(0xE2),
        static_cast<char>(0x9C),
        static_cast<char>(0x94),
        0
    };
};
} // namespace mc
