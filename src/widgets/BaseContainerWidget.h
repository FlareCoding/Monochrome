#pragma once
#include "BaseWidget.h"

namespace mc
{
    class BaseContainerWidget : public BaseWidget
    {
    public:
        BaseContainerWidget();
        virtual ~BaseContainerWidget() = default;

        // Specifies that the following widget is a container and can hold children widgets
        inline bool isContainer() const override { return true; }

        // Adds a child widget to the list of widgets
        // @param child Widget to be added
        void addChild(Shared<BaseWidget> child);

        // Inserts a child at a specified index
        // @param child Widget to be inserted
        // @param index Index in the list of children at which to insert the child
        void insertChild(Shared<BaseWidget> child, uint64_t index);

        // Removes a child widget from the list of children
        // @param child Widget to be removed
        // @returns Status of whether a child widget has been removed successfully
        bool removeChild(Shared<BaseWidget> child);

        // Removes a child widget from the list of children
        // @param uuid ID of the child widget to remove
        // @returns Status of whether a child widget has been removed successfully
        bool removeChild(uuid_t uuid);

        // Removes all child widgets
        void removeAllChildren();

        // Attemts to find a child widget given its ID
        // @param uuid ID of the child widget to find
        // @returns Shared pointer to the child widget if it was found, nullptr otherwise
        Shared<BaseWidget> findChild(uuid_t uuid);

        // Attemts to find a child widget given its index in the list of children
        // @param uuid Index in the list of children of the child widget
        // @returns Shared pointer to the child widget if it was found, nullptr otherwise
        Shared<BaseWidget> getChild(uint64_t index);

        // @returns A list of all direct children widgets
        inline std::vector<Shared<BaseWidget>>& getChildren() { return d_children; }

    protected:
        std::vector<Shared<BaseWidget>> d_children;
    };
}
