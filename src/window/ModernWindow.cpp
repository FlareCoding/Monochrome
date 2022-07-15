#include "ModernWindow.h"
#include <core/InternalFlags.h>

#define MC_MODERNWINDOW_FLAGS  (1ULL << WindowStyleBorderless) |  \
                                (1ULL << WindowStyleResizable) |  \
                                (1ULL << WindowStyleDraggable) |  \
                                (1ULL << WindowStyleMinimizable)

namespace mc {
    ModernWindow::ModernWindow(uint32_t width, uint32_t height, const std::string& title)
        : UIWindow(MC_MODERNWINDOW_FLAGS, width, height, title) {
        // Create the 'close', 'minimize', and 'maximize'
        // buttons as well as the title label.
        _createDecoratorWidgets();

        // Setup events handlers
        _setupEventHandlers();
    }

    void ModernWindow::setBackgroundColor(const Color& color) {
        UIWindow::setBackgroundColor(color);

        // Blend the decorator widgets with the background
        d_closeButton->backgroundColor = getBackgroundColor();
        d_maximizeButton->backgroundColor = getBackgroundColor();
        d_minimizeButton->backgroundColor = getBackgroundColor();
    }

    void ModernWindow::setTitle(const std::string& title) {
        UIWindow::setTitle(title);

        // Modify the title label
        d_titleLabel->text = title;
    }

    void ModernWindow::_createDecoratorWidgets() {
        // Create the 'close' button
        d_closeButton = _createDecoratorButton();
        d_closeButton->position = { (int32_t)getWidth() - (int32_t)d_decoratorWidgetWidth, 0 };
        d_closeButton->text = "x";
        d_closeButton->onHoverColor = Color(140, 20, 20);
        d_closeButton->on("clicked", [this](Shared<Event> e) {
            this->close();
        });

        // Add the 'close' button to the window
        addWidget(d_closeButton);

        // Create the 'maximize' button
        d_maximizeButton = _createDecoratorButton();
        d_maximizeButton->position = {
            (int32_t)getWidth() - (int32_t)d_decoratorWidgetWidth * 2, 0
        };
        d_maximizeButton->text = "▢";
        d_maximizeButton->fontSize = 11;
        d_maximizeButton->on("clicked", [this](Shared<Event> e) {
            if (d_maximized) {
                this->restoreMaximize();
                d_maximized = false;
            } else {
                this->maximize();
                d_maximized = true;
            }
        });

        // Add the 'maximize' button to the window
        addWidget(d_maximizeButton);

        // Create the 'minimize' button
        d_minimizeButton = _createDecoratorButton();
        d_minimizeButton->position = {
            (int32_t)getWidth() - (int32_t)d_decoratorWidgetWidth * 3, 0
        };
        d_minimizeButton->text = "_";
        d_minimizeButton->fontSize = 10;
        d_minimizeButton->on("clicked", [this](Shared<Event> e) {
            this->minimize();
        });

        // Add the 'minimize' button to the window
        addWidget(d_minimizeButton);

        // Create the 'title' label
        d_titleLabel = MakeRef<Label>();
        d_titleLabel->position = { 10, 6 };
        d_titleLabel->size = { 400, 28 };
        d_titleLabel->color = Color::white;
        d_titleLabel->alignment = "left";
        d_titleLabel->text = getTitle();

        // Add the 'title' label to the window
        addWidget(d_titleLabel);
    }

    void ModernWindow::_setupEventHandlers() {
        on("sizeChanged", [this](Shared<Event> event) {
            auto width = event->get<uint32_t>("width");
            auto height = event->get<uint32_t>("height");

            d_closeButton->position = { (int32_t)getWidth() - (int32_t)d_decoratorWidgetWidth, 0 };
            d_maximizeButton->position = {
                (int32_t)getWidth() - (int32_t)d_decoratorWidgetWidth * 2, 0
            };
            d_minimizeButton->position = {
                (int32_t)getWidth() - (int32_t)d_decoratorWidgetWidth * 3, 0
            };
        });
    }

    Shared<Button> ModernWindow::_createDecoratorButton() {
        auto decoratorButton = MakeRef<Button>();
        decoratorButton->size = { d_decoratorWidgetWidth, d_decoratorWidgetHeight };
        decoratorButton->cornerRadius = 0;
        decoratorButton->backgroundColor = getBackgroundColor();
        decoratorButton->color = Color::white;

        return decoratorButton;
    }
} // namespace mc
