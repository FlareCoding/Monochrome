#pragma once
#include <widgets/AllWidgets.h>

namespace mc::mcstudio {
class DockAnchorPropertyField : public StackPanel {
public:
    DockAnchorPropertyField(
        const std::string& name,
        const std::string& value
    );

private:
    Shared<Button> d_dockControlTopButton;
    Shared<Button> d_dockControlBottomButton;
    Shared<Button> d_dockControlLeftButton;
    Shared<Button> d_dockControlRightButton;
    Shared<Button> d_dockControlFillButton;

    const Color d_dimmedDockButtonColor = Color(70, 70, 70);
    const Color d_selectedDockButtonColor = Color(130, 130, 130);

private:
    std::string d_propName;
    std::string d_propValue;

    void _init();

    Shared<Button> _createControlButton(DockAnchor anchor);

    void _dimAllControlButtons();
    void _highlightNecessaryControlButton();

    void _controlButton_OnClick(Shared<Event> e);
};
} // namespace mc::mcstudio
