#include "Overlay.h"
#include <core/InternalFlags.h>
#include <utils/PlacementConstraintSystem.h>

#define MC_OVERLAYWINDOW_FLAGS (1ULL << WindowStyleBorderless)

namespace mc
{
	Overlay::Overlay() {
		autoHide = true;
		spawnDirection = OverflowDirection::Down;

		d_overlayWindow = MakeRef<UIWindow>(MC_OVERLAYWINDOW_FLAGS, 1, 1);
		d_overlayWindow->hide();
		d_overlayWindow->setPosition(d_anchorPoint);

		d_overlayWindow->on("globalMouseDown", [this](Shared<Event> e) {
			if (!autoHide || !d_overlayOpened) {
				return;
			}

			// Get the mouse location where the mouse down event occured
			auto clickEvent = std::static_pointer_cast<MouseButtonEvent>(e);
			auto clickLocation = clickEvent->getLocation();
			
			// Determine if the overlay should close
			// depending on where the mouse was clicked.
			auto mouseDownInsideOverlay = _isMouseClickedInOverlay(clickLocation);

			if (!mouseDownInsideOverlay) {
				hide();
			}
		});
	}
	
	Overlay::~Overlay() {
		d_overlayWindow->close();
	}

	void Overlay::setSize(uint32_t width, uint32_t height) {
		d_overlayWindow->setSize(width, height);
	}

	void Overlay::setSize(const Size& size) {
		d_overlayWindow->setSize(size.width, size.height);
	}

	void Overlay::show() {
		if (d_overlayOpened) {
			return;
		}

		d_overlayWindow->setPosition(d_anchorPoint);
		d_overlayWindow->show();
		d_overlayWindow->focus();

		d_overlayOpened = true;
	}

	void Overlay::hide() {
		d_overlayOpened = false;
		d_overlayWindow->hide();

		// Get the virtual screen container
		// from the placement constraint system.
		auto screenContainer = utils::PlacementConstraintSystem::getContainer(MAIN_SCREEN_CONTAINER_NAME);

		// Unregister the overal window from the
		// global container placement system.
		screenContainer->removeChild(
			reinterpret_cast<uint64_t>(this) // ID
		);
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

	void Overlay::setActivatorWidget(BaseWidget* widget) {
		// Check if the previous activator widget needs to be detached
		if (d_activatorWidget) {
			d_activatorWidget->off("clicked");
		}

		d_activatorWidget = widget;

		// If widget is nullptr, treat it as
		// resetting and removing the activator widget.
		if (!d_activatorWidget) {
			return;
		}

		d_activatorWidget->on("clicked", [this](Shared<Event> e) {
			auto clickEvent = std::static_pointer_cast<MouseButtonEvent>(e);
			if (clickEvent->getButton() != MouseButton::Left) {
				return;
			}

			// Calculate the anchor position
			// according to the anchor point.
			auto anchorPoint = _calculateAnchorPosition(clickEvent);

			// Set the overlay's anchor point to be
			// right below the activator widget.
			setAnchor(anchorPoint);

			// Open the overlay
			show();
		});
	}
	
	void Overlay::addChildOverlay(Shared<Overlay> overlay) {
		d_childOverlays.push_back(overlay);
	}

	Position Overlay::_calculateAnchorPosition(Shared<MouseButtonEvent> e) {
		// Get the anchor widget's position in the window
		auto anchorPositionInWindow = d_activatorWidget->getPositionInWindow();

		// Get click event's position information
		auto clickPos = e->getLocation();
		auto screenClickPos = e->getScreenLocation();

		// Calculate click offset from the anchor's origin
		Position anchorPosDiff = {
			clickPos.x - anchorPositionInWindow.x,
			clickPos.y - anchorPositionInWindow.y
		};

		// Calculate the anchor's origin
		Point anchorOrigin = {
			screenClickPos.x - anchorPosDiff.x,
			screenClickPos.y - anchorPosDiff.y
		};

		// Get the virtual screen container
		// from the placement constraint system.
		auto screenContainer = utils::PlacementConstraintSystem::getContainer(MAIN_SCREEN_CONTAINER_NAME);

		// Calculate the anchor point according
		// to the overlay's preferred spawn position.
		auto anchorPosition = screenContainer->insertChild(
			reinterpret_cast<uint64_t>(this), // ID
			Size(d_overlayWindow->getWidth(), d_overlayWindow->getHeight()),
			anchorOrigin,
			d_activatorWidget->size,
			spawnDirection
		);

		return anchorPosition;
	}
	
	bool Overlay::_isMouseClickedInOverlay(const Position& clickPosition) {
		auto windowFrame = Frame(d_overlayWindow->getPosition(), d_overlayWindow->getSize());
		auto clickedInsideOverlay = windowFrame.containsPoint(clickPosition);

		if (clickedInsideOverlay) {
			return true;
		}

		// Check if the mouse was clicked in the child overlay
		if (d_childOverlays.size()) {
			for (auto& child : d_childOverlays) {
				if (child->_isMouseClickedInOverlay(clickPosition)) {
					return true;
				}
			}
		}

		// Otherwise return false
		return false;
	}
}
