#include "Overlay.h"
#include <core/InternalFlags.h>

#define MC_OVERLAYWINDOW_FLAGS (1ULL << WindowStyleBorderless)

namespace mc
{
	Overlay::Overlay() {
		d_overlayWindow = MakeRef<UIWindow>(MC_OVERLAYWINDOW_FLAGS, 1, 1);
		d_overlayWindow->hide();
		d_overlayWindow->setPosition(d_anchorPoint);
		d_overlayWindow->forwardEmittedEvents(this);

		on("focusChanged", [this](Shared<Event> e) {
			if (autoHide) {
				bool focused = e->get<bool>("focused");
				if (!focused) {
					d_overlayWindow->hide();
				}
			}
		});
	}
	
	Overlay::~Overlay() {
		d_overlayWindow->close();
	}

	void Overlay::setSize(uint32_t width, uint32_t height) {
		d_overlayWindow->setSize(width, height);
	}

	void Overlay::show() {
		d_overlayWindow->setPosition(d_anchorPoint);
		d_overlayWindow->show();
		d_overlayWindow->focus();
	}

	void Overlay::hide() {
		if (autoHide) {
			d_overlayWindow->unfocus();
		}
		else {
			d_overlayWindow->hide();
		}
	}

	void Overlay::setContent(Shared<BaseWidget> content) {
		d_overlayWindow->removeAllWidgets();
	    d_overlayWindow->addWidget(content);
	}

	Color Overlay::getBackgroundColor() {
		return d_overlayWindow->getBackgroundColor();
	}
	
	void Overlay::setBackgroundColor(const Color& color) {
		d_overlayWindow->setBackgroundColor(color);
	}

	void Overlay::setAnchor(const Point& point) {
	    d_anchorPoint = point;
	}

	void Overlay::setActivatorWidget(Shared<BaseWidget> widget) {
		// Check if the previous activator widget needs to be detached
		if (d_activatorWidget) {
			d_activatorWidget->off("clicked");
		}

		d_activatorWidget = widget;

		d_activatorWidget->on("clicked", [this](Shared<Event> e) {
			auto clickEvent = std::static_pointer_cast<MouseButtonEvent>(e);
			if (clickEvent->getButton() != MouseButton::Left) {
				return;
			}

			// Get the anchor widget's position in the window
			auto anchorPositionInWindow = d_activatorWidget->getPositionInWindow();

			// Get click event's position information
			auto clickPos = clickEvent->getLocation();
			auto screenClickPos = clickEvent->getScreenLocation();

			// Calculate click offset from the anchor's origin
			Position anchorPosDiff = {
				clickPos.x - anchorPositionInWindow.x,
				clickPos.y - anchorPositionInWindow.y
			};

			// Calculate the new anchor point
			Point anchorPoint = {
				screenClickPos.x - anchorPosDiff.x,
				screenClickPos.y - anchorPosDiff.y + (int32_t)d_activatorWidget->size->height
			};

			// Set the overlay's anchor point to be
			// right below the activator widget.
			setAnchor(anchorPoint);

			// Open the overlay
			show();
		});
	}
}
