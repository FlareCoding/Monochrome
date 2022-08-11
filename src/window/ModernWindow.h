#pragma once
#include "UIWindow.h"
// #include <widgets/Button.h>
// #include <widgets/Label.h>

namespace mc {
class ModernWindow : public UIWindow {
public:
    explicit ModernWindow(
        uint32_t width = 800,
        uint32_t height = 600,
        const std::string& title = "Window"
    );

    void setBackgroundColor(const Color& color) override;
    void setTitle(const std::string& title) override;

private:
    uint32_t d_decoratorWidgetWidth = 36;
    uint32_t d_decoratorWidgetHeight = 26;

    // Shared<Label> d_titleLabel;
    // Shared<Button> d_closeButton;
    // Shared<Button> d_minimizeButton;
    // Shared<Button> d_maximizeButton;
    bool d_maximized = false;

    void _createDecoratorWidgets();
    void _setupEventHandlers();

    // Factory for a decorator widget
    // Shared<Button> _createDecoratorButton();
};
} // namespace mc
