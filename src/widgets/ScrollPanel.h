#pragma once
#include "Button.h"
#include "StackPanel.h"
#include "visuals/RectVisual.h"

namespace mc {
class ScrollPanel : public BaseContainerWidget {
public:
    ScrollPanel();

    // Corner radius of the panel
    PropertyObserver<uint32_t>  cornerRadius;

    // Color of the tracks that contain
    // vertical and horizontal scrollbars.
    PropertyObserver<Color>     scrollbarTracksColor;

    // Background color of the buttons on the scrollbar tracks
    PropertyObserver<Color>     scrollbarTrackButtonBackground;

    // Arrow color of the buttons on the scrollbar tracks
    PropertyObserver<Color>     scrollbarTrackButtonColor;

    // Color of the scrollbars
    PropertyObserver<Color>     scrollbarColor;

    // Indicates whether or not the content should automatically
    // be scrolled to the bottom on content change events.
    PropertyObserver<bool>      autoscroll;

    void scrollContentVertically(int32_t amount);
    void scrollContentHorizontally(int32_t amount);

protected:
    Size _measureSize() override;
    void _onArrangeChildren() override;

private:
    void _setupProperties();

    Shared<StackPanel> d_verticalScrollbarTrack;
    Shared<Button> d_verticalTrackUpButton;
    Shared<Button> d_verticalTrackDownButton;
    Shared<Button> d_verticalScrollbar;

    Shared<StackPanel> d_horizontalScrollbarTrack;
    Shared<Button> d_horizontalTrackLeftButton;
    Shared<Button> d_horizontalTrackRightButton;
    Shared<Button> d_horizontalScrollbar;

    void _clampContentPosition();

    uint32_t _calculateVerticalScrollbarSize();
    void _calculateVerticalScrollbarPosition();

    uint32_t _calculateHorizontalScrollbarSize();
    void _calculateHorizontalScrollbarPosition();

private:
    bool        d_verticalScrollbarPressed = false;
    bool        d_horizontalScrollbarPressed = false;
    Position    d_mousePressLocation = Point(0, 0);
    Position    d_positionInWindow = Point(0, 0);
    int32_t     d_preScrollContentPosition = 0;

    void _verticalScrollbarOnMouseDown(Shared<Event> e);
    void _verticalScrollbarOnMouseUp(Shared<Event> e);
    void _verticalScrollbarOnMouseMoved(Shared<Event> e);

    void _horizontalScrollbarOnMouseDown(Shared<Event> e);
    void _horizontalScrollbarOnMouseUp(Shared<Event> e);
    void _horizontalScrollbarOnMouseMoved(Shared<Event> e);

    void _showVerticalScrollElements();
    void _hideVerticalScrollElements();

    void _showHorizontalScrollElements();
    void _hideHorizontalScrollElements();

    float       _getVerticalScrollbarScrolledPercentage();
    uint32_t    _getVerticalScrollbarMovableDistance();
    uint32_t    _getMaxPossibleVerticalScroll();

    float       _getHorizontalScrollbarScrolledPercentage();
    uint32_t    _getHorizontalScrollbarMovableDistance();
    uint32_t    _getMaxPossibleHorizontalScroll();

private:
    const uint64_t d_privateWidgets = 8; // Scroll track elements + scrollbars
    const uint32_t d_scrollbarTrackSize = 14;
    const int32_t  d_scrollWheelSensitivity = 10;
};
} // namespace mc
