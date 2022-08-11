#include "Overlay.h"
#include <core/InternalFlags.h>
#include <utils/PlacementConstraintSystem.h>

#define MC_OVERLAYWINDOW_FLAGS (1ULL << WindowStyleBorderless)

namespace mc {
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
        auto screenContainer =
            utils::PlacementConstraintSystem::getContainer(MAIN_SCREEN_CONTAINER_NAME);

        // Unregister the overal window from the
        // global container placement system.
        screenContainer->removeChild(
            reinterpret_cast<uint64_t>(this) // ID
        );
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

    void Overlay::addChildOverlay(Shared<Overlay> overlay) {
        d_childOverlays.push_back(overlay);
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
} // namespace mc
