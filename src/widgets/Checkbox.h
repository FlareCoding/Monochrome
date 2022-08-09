#pragma once
#include "Label.h"
#include "Button.h"

namespace mc {
class Checkbox : public BaseWidget {
public:
    Checkbox();

    Shared<Button> button;
    Shared<Label> label;

protected:
    Size _measureSize() override;
    void _onArrangeChildren() override;

private:
    void _setupProperties();
};
} // namespace mc
