#pragma once
#include <widgets/AllWidgets.h>

namespace mc::mcstudio {
class AlignmentPropertyField : public StackPanel {
public:
    AlignmentPropertyField(
        const std::string& name,
        const std::string& value
    );

private:
    std::string d_propName;
    std::string d_propValue;

    void _init();

    Shared<Button> _createAlignmentImgButton(
        const std::string& alignment,
        uint32_t orderIdx
    );

    void _alignmentControlButton_OnClick(Shared<Event> e);
};
} // namespace mc::mcstudio
