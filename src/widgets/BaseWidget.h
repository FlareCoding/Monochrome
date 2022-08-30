#pragma once
#include "WidgetContainer.h"
#include <utils/uuid.h>
#include <utils/Cursor.h>
#include "visuals/VisualElement.h"

#define NOT_SET 0

namespace mc {
enum HorizontalAlignment {
    HALeft,
    HARight,
    HACenter,
    HAFill
};

enum VerticalAlignment{
    VATop,
    VABottom,
    VACenter,
    VAFill
};

class BaseWidget : public PrivateWidgetContainer {
friend class Renderer;
friend class EventProcessor;

public:
    BaseWidget();
    virtual ~BaseWidget() = default;

    virtual bool isContainer() const { return false; }

    Position getPositionInWindow() const;

    inline uuid_t getID() const { return d_uuid; }
    inline uint64_t& getInternalFlags() { return d_internalFlags; }

    PropertyObserver<uint32_t>      zIndex;
    PropertyObserver<bool>          visible;
    PropertyObserver<bool>          focused;
    PropertyObserver<CursorType>    cursorType;

    PropertyObserver<Position>      position;

    PropertyObserver<uint32_t>      fixedWidth;
    PropertyObserver<uint32_t>      minWidth;
    PropertyObserver<uint32_t>      maxWidth;

    PropertyObserver<uint32_t>      fixedHeight;
    PropertyObserver<uint32_t>      minHeight;
    PropertyObserver<uint32_t>      maxHeight;

    PropertyObserver<uint32_t>      marginTop;
    PropertyObserver<uint32_t>      marginBottom;
    PropertyObserver<uint32_t>      marginLeft;
    PropertyObserver<uint32_t>      marginRight;

    PropertyObserver<HorizontalAlignment>   horizontalAlignment;
    PropertyObserver<VerticalAlignment>     verticalAlignment;

    inline void setParent(BaseWidget* parent) { d_parent = parent; }
    inline BaseWidget* getParent() const { return d_parent; }

    // Requests focus to be set to this widget
    void focus();

    // Requests focus to be removed from this widget
    void unfocus();

    // Used to retrieve the currently marked
    // size that the widget prefers to have.
    Size getDesiredSize();

    // Returns the desired size of the widget in
    // addition to the margins around the widget.
    Size getDesiredSizeWithMargins();

    // Returns the final computed size of the widget
    Size getComputedSize();

    // Returns the final computed size of the
    // widget including the surrounding margins.
    Size getComputedSizeWithMargins();

    // Recursively descends down the widget tree
    // and calculates each widget's desired size
    // according to the internal _measuredSize override.
    void measure();

    // Recursively arranges the children using the
    // internal _onArrangeChildren() override.
    //
    // *Note* By the time _onArrangeChildren() is called,
    // the computed size of the widget is already set.
    void arrangeChildren();

    // Directly sets the final computed size of the widget
    void setComputedSize(const Size& size);

    // Returns whether or not a layout recalculation
    // should occur after a layout-dependent property update.
    inline bool isLayoutDirty() const { return d_isLayoutDirty; }

    // Forcefully marks the current widget's layout state
    // as dirty, forcing a redraw with a layout recalculation.
    void markLayoutDirty();

    // Sets the internal IsDraggable flag on.
    // Use only when you need to process mouseUp
    // events when mouse is outside of the widget.
    void markMouseDraggable();

protected:
    BaseWidget* d_parent = nullptr;

    void addCoreVisualElement(Shared<VisualElement> visual);
    void addOverlayVisualElement(Shared<VisualElement> visual);

    virtual void _onSetComputedSize(const Size& size) {}

    // Called during the measure() routine during
    // the measure phase of the layout system.
    // @returns Minimum required size to contain all children
    virtual Size _measureSize() = 0;

    // Called during the arrangeChildren() routine during
    // the arrange phase of the layout system. Responsible
    // for arranging the children and setting their computed sizes.
    virtual void _onArrangeChildren() {}

private:
    Size d_desiredSize = Size(0, 0);
    Size d_computedSize = Size(0, 0);

    bool d_isLayoutDirty = true;

private:
    uuid_t d_uuid;
    uint64_t d_internalFlags = 0;

    // Visual elements that the widget is comprised of
    // and get rendered first before any children widgets.
    std::vector<Shared<VisualElement>> d_coreVisualElements;

    // Visual elements that the widget that act as an overlay
    // and get rendered first before any children widgets.
    std::vector<Shared<VisualElement>> d_overlayVisualElements;

private:
    void _setDefaultProperties();
};

class BaseContainerWidget : public BaseWidget {
public:
    BaseContainerWidget() = default;
    virtual ~BaseContainerWidget() = default;

    // Adds a child to the list of widgets
    // @param child Child element to be added
    void addChild(Shared<BaseWidget> child);

    // Removes a child from the list of children
    // @param child Child element to be removed
    // @returns Status of whether a child has been removed successfully
    bool removeChild(Shared<BaseWidget> child);

    // Removes a child from the list of children
    // @param uuid ID of the child to remove
    // @returns Status of whether a child has been removed successfully
    bool removeChild(uuid_t uuid);

    // Removes all children
    void removeAllChildren();

    // Attemts to find a child given its ID
    // @param uuid ID of the child to find
    // @returns Shared pointer to the child if it was found, nullptr otherwise
    Shared<BaseWidget> findChild(uuid_t uuid);

    // Attemts to find a child given its index in the list of children
    // @param uuid Index in the list of children of the child widget
    // @returns Shared pointer to the child if it was found, nullptr otherwise
    Shared<BaseWidget> getChild(uint64_t index);

    // @returns A list of all direct children widgets
    std::vector<Shared<BaseWidget>>& getChildren();
};
} // namespace mc
