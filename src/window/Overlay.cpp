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
}
