#include "DockAnchorPropertyField.h"

namespace mc::mcstudio {
	DockAnchorPropertyField::DockAnchorPropertyField(
		const std::string& name,
		const std::string& value
	) : StackPanel() {
		d_propName = name;
		d_propValue = value;

		appendAllowedEvent("widgetPropertyModified");

		_init();
	}

	void DockAnchorPropertyField::_init() {
        this->backgroundColor = Color::transparent;
        this->marginBottom = 4;
        this->marginLeft = 10;

        auto propertyNameLabel = MakeRef<Label>();
        propertyNameLabel->text = "Dock Anchor";
        propertyNameLabel->fixedWidth = 260;
        propertyNameLabel->fixedHeight = 10;
        propertyNameLabel->marginBottom = 6;
        addChildOffline(propertyNameLabel);

        d_dockControlTopButton      = _createControlButton(DockAnchor::Top);
        d_dockControlBottomButton   = _createControlButton(DockAnchor::Bottom);
        d_dockControlLeftButton     = _createControlButton(DockAnchor::Left);
        d_dockControlRightButton    = _createControlButton(DockAnchor::Right);
        d_dockControlFillButton     = _createControlButton(DockAnchor::Fill);

        auto controlDockPanel = MakeRef<DockPanel>();
        controlDockPanel->horizontalAlignment = HACenter;
        controlDockPanel->backgroundColor = Color::red;
        controlDockPanel->fixedWidth = 160;
        controlDockPanel->fixedHeight = 80;
        controlDockPanel->backgroundColor = Color::transparent;
        controlDockPanel->addChildOffline(d_dockControlTopButton);
        controlDockPanel->addChildOffline(d_dockControlBottomButton);
        controlDockPanel->addChildOffline(d_dockControlLeftButton);
        controlDockPanel->addChildOffline(d_dockControlRightButton);
        controlDockPanel->addChildOffline(d_dockControlFillButton);
        addChildOffline(controlDockPanel);

        // Apply appropriate coloring to the control buttons
        _highlightNecessaryControlButton();
	}

    Shared<Button> DockAnchorPropertyField::_createControlButton(DockAnchor anchor) {
        auto dockControlButton = MakeRef<Button>();
        dockControlButton->borderColor = Color(10, 10, 10);
        dockControlButton->borderThickness = 1;
        dockControlButton->cornerRadius = 0;
        dockControlButton->label->color = Color::white;
        dockControlButton->label->text = "";
        dockControlButton->dockAnchor = anchor;
        dockControlButton->on("clicked", &DockAnchorPropertyField::_controlButton_OnClick, this);

        switch (anchor) {
        case DockAnchor::Top:
        case DockAnchor::Bottom: {
            dockControlButton->fixedHeight = 16;
            break;
        }
        case DockAnchor::Left:
        case DockAnchor::Right: {
            dockControlButton->fixedWidth = 20;
            break;
        }
        default: break;
        }

        return dockControlButton;
    }

    void DockAnchorPropertyField::_dimAllControlButtons() {
        d_dockControlTopButton->backgroundColor = d_dimmedDockButtonColor;
        d_dockControlBottomButton->backgroundColor = d_dimmedDockButtonColor;
        d_dockControlLeftButton->backgroundColor = d_dimmedDockButtonColor;
        d_dockControlRightButton->backgroundColor = d_dimmedDockButtonColor;
        d_dockControlFillButton->backgroundColor = d_dimmedDockButtonColor;
    }

    void DockAnchorPropertyField::_highlightNecessaryControlButton() {
        // Dim all buttons first to highlight only one later
        _dimAllControlButtons();

        if (d_propValue == "top") {
            d_dockControlTopButton->backgroundColor = d_selectedDockButtonColor;

        } else if (d_propValue == "bottom") {
            d_dockControlBottomButton->backgroundColor = d_selectedDockButtonColor;

        } else if (d_propValue == "left") {
            d_dockControlLeftButton->backgroundColor = d_selectedDockButtonColor;

        } else if (d_propValue == "right") {
            d_dockControlRightButton->backgroundColor = d_selectedDockButtonColor;

        } else {
            d_dockControlFillButton->backgroundColor = d_selectedDockButtonColor;
        }
    }

    void DockAnchorPropertyField::_controlButton_OnClick(Shared<Event> e) {
        auto selectedAnchor = e->target->dockAnchor;

        switch (selectedAnchor) {
        case DockAnchor::Top: {
            d_propValue = "top";
            break;
        }
        case DockAnchor::Bottom: {
            d_propValue = "bottom";
            break;
        }
        case DockAnchor::Left: {
            d_propValue = "left";
            break;
        }
        case DockAnchor::Right: {
            d_propValue = "right";
            break;
        }
        case DockAnchor::Fill: {
            d_propValue = "fill";
            break;
        }
        default: break;
        }

        // Highlight the selected control button
        _highlightNecessaryControlButton();
        
        fireEvent("widgetPropertyModified", {
            { "propName", d_propName },
            { "propValue", d_propValue },
        });
    }
} // namespace mc::mcstudio
