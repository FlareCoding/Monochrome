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
    PropertyObserver<Size>          size;
    PropertyObserver<Size>          minSize;
    PropertyObserver<Size>          maxSize;
    PropertyObserver<uint32_t>      marginTop;
    PropertyObserver<uint32_t>      marginBottom;
    PropertyObserver<uint32_t>      marginLeft;
    PropertyObserver<uint32_t>      marginRight;

    PropertyObserver<HorizontalAlignment>   horizontalAlignment;
    PropertyObserver<VerticalAlignment>     verticalAlignment;

    inline void setParent(BaseWidget* parent) { d_parent = parent; }
    inline BaseWidget* getParent() const { return d_parent; }

    Size getClientSize();

protected:
    BaseWidget* d_parent = nullptr;

    void addCoreVisualElement(Shared<VisualElement> visual);
    void addOverlayVisualElement(Shared<VisualElement> visual);

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
