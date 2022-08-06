#include "StackPanel.h"
#include "visuals/Visuals.h"

namespace mc {
	StackPanel::StackPanel() {
		_createVisuals();
		_setupProperties();
	}

	void StackPanel::_createVisuals() {
		// Setup the panel's body rectangle
		auto bodyRect = MakeRef<RectVisual>();

		size.forwardAssignment(&bodyRect->size);
		cornerRadius.forwardAssignment(&bodyRect->cornerRadius);
		backgroundColor.forwardAssignment(&bodyRect->color);
		addCoreVisualElement(bodyRect);
	}

	void StackPanel::_setupProperties() {
		backgroundColor = Color::lightGray;
		cornerRadius = 2;

		on("childAdded", [this](Shared<Event> e) {
			auto child = e->get<BaseWidget*>("child");
			_onChildAdded(child);
		});

		on("childRemoved", [this](Shared<Event> e) {
			auto child = e->get<BaseWidget*>("child");
			_onChildRemoved(child);
		});
	}

	void StackPanel::_onChildAdded(BaseWidget* child) {
		auto widgetSize = child->getClientSize();

		auto xPos = 0;
		auto yPos = nextAvailableCellPosition.y + static_cast<int32_t>(child->marginTop);

		switch (child->horizontalAlignment) {
		case HorizontalAlignment::HALeft: {
			xPos = static_cast<int32_t>(child->marginLeft);
			break;
		}
		case HorizontalAlignment::HARight: {
			xPos = static_cast<int32_t>(size->width - child->marginRight - child->size->width);
			break;
		}
		case HorizontalAlignment::HACenter: {
			xPos = static_cast<int32_t>((size->width / 2) - (widgetSize.width / 2));
			break;
		}
		case HorizontalAlignment::HAFill: {
			xPos = static_cast<int32_t>(child->marginLeft);
			
			// Stretch the size of the child to fit into the entire cell
			auto childMargins = child->marginLeft + child->marginRight;

			child->size = {
				size->width - childMargins,
				child->size->height
			};
			break;
		}
		default: break;
		}

		child->position->x = xPos;
		child->position->y = yPos;
		
		nextAvailableCellPosition.y += widgetSize.height;
	}

	void StackPanel::_onChildRemoved(BaseWidget* child) {
		// TO-DO
	}
} // namespace mc
