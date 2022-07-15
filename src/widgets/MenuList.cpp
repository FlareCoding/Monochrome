#include "MenuList.h"
#include <window/Overlay.h>

#define PROPAGATE_PROPERTY_TO_MENU_ITEMS_X(thisProp, childProp, setValue) \
     this->thisProp.on("propertyChanged", [this](auto e) { \
        for (auto& [name, menuItem] : d_menuItems) { \
            auto& menuButton = menuItem.first; \
            menuButton->childProp = setValue; \
        } \
     });

#define PROPAGATE_PROPERTY_TO_MENU_ITEMS(thisProp, setValue) \
        PROPAGATE_PROPERTY_TO_MENU_ITEMS_X(thisProp, thisProp, setValue)

namespace mc {
    MenuList::MenuList() {
        _setupProperties();
    }

    MenuList::MenuList(const std::string& name) : name(name) {
        _setupProperties();
    }

    void MenuList::_setupProperties() {
        name.forwardEmittedEvents(this);

        borderColor = Color::white;
        borderColor.forwardEmittedEvents(this);

        borderThickness = 1;
        borderThickness.forwardEmittedEvents(this);

        menuItemSize = 24;
        menuItemSize.forwardEmittedEvents(this);

        spawnDirection = OverflowDirection::Right;
        spawnDirection.forwardEmittedEvents(this);

        backgroundColor = Color(60, 60, 60);
        color = Color::white;

        size.on("propertyChanged", [this](auto e) {
            auto overlayHeight = (uint32_t)d_menuItems.size() *
                                menuItemSize + borderThickness * 2;

            // Automatically adjusting the height of the overlay
            this->size->height = overlayHeight;

            d_overlay->setSize(size);
            d_contentPanel->size = size;
            d_overlayBorder->size = size;

            // Recalculate sizes for the child menu items
            uint32_t itemIndex = 0;
            for (auto& [name, menuItem] : d_menuItems) {
                auto& menuButton = menuItem.first;
                menuButton->size = {
                    size->width - borderThickness * 2,
                    menuItemSize
                };

                ++itemIndex;
            }
        });

        backgroundColor.on("propertyChanged", [this](auto e) {
            // Changing the background color of the host panel
            d_contentPanel->backgroundColor = backgroundColor;
        });

        borderColor.on("propertyChanged", [this](auto e) {
            // Changing the border color of the overlay's border panel
            d_overlayBorder->backgroundColor = borderColor;
        });

        borderThickness.on("propertyChanged", [this](auto e) {
            // Changing the thickness of the overlay's border panel
            d_overlayBorder->stroke = borderThickness;
        });

        borderThickness.on("propertyChanged", [this](auto e) {
            // Recalculating position and size of each menu item
            _recalculateMenuItemBounds();
        });

        menuItemSize.on("propertyChanged", [this](auto e) {
            // Recalculating position and size of each menu item
            _recalculateMenuItemBounds();
        });

        spawnDirection.on("propertyChanged", [this](auto e) {
            // Set the overlays spawn direction
            d_overlay->spawnDirection = spawnDirection;
        });

        // The following callbacks are there to ensure that
        // whenever certain local attributes change, the same
        // attributes for the child menu items also apply.
        PROPAGATE_PROPERTY_TO_MENU_ITEMS(backgroundColor, backgroundColor);
        PROPAGATE_PROPERTY_TO_MENU_ITEMS(color, color);

        // Create the content panel
        d_contentPanel = MakeRef<Panel>();
        d_contentPanel->cornerRadius = 0;
        d_contentPanel->backgroundColor = Color::black;

        d_overlayBorder = MakeRef<Panel>();
        d_overlayBorder->position = { 0, 0 };
        d_overlayBorder->size = size;
        d_overlayBorder->backgroundColor = Color::white;
        d_overlayBorder->filled = false;
        d_overlayBorder->stroke = borderThickness;
        d_overlayBorder->cornerRadius = 0;
        d_contentPanel->addChild(d_overlayBorder);

        // Create the overlay
        d_overlay = MakeRef<Overlay>();
        d_overlay->spawnDirection = spawnDirection;
        d_overlay->setContent(d_contentPanel);

        // Setup a default size
        size = { 140, 0 };

        // Allow MenuList specific events
        appendAllowedEvent("itemSelected");
        appendAllowedEvent("overlayShouldClose");

        // Hide the overlay when needed
        on("overlayShouldClose", [this](auto e) {
            d_overlay->hide();
        });
    }

    void MenuList::_recalculateMenuItemBounds() {
        uint32_t itemIndex = 0;
        for (auto& [name, menuItem] : d_menuItems) {
            auto& menuButton = menuItem.first;

            menuButton->position = {
                (int32_t)borderThickness,
                (int32_t)(itemIndex * menuItemSize + borderThickness)
            };
            menuButton->size = {
                size->width - borderThickness * 2,
                menuItemSize
            };

            ++itemIndex;
        }

        // Adjust the overall size of the menulist display
        this->size = {
            size->width,
            (uint32_t)d_menuItems.size() * menuItemSize + borderThickness * 2
        };
    }

    void MenuList::addSubMenu(Shared<MenuList> menu) {
        auto menuButton = MakeRef<Button>();
        menuButton->cornerRadius = 0;
        menuButton->text = menu->name;
        menuButton->color = color;
        menuButton->backgroundColor = backgroundColor;
        d_contentPanel->addChild(menuButton);

        // Set the button as menu list activator
        menu->setActivatorWidget(menuButton.get());

        // Set the menu's overlay as the child overlay
        d_overlay->addChildOverlay(menu->d_overlay);

        // Add the menu to the list
        d_menuItems.push_back({ menu->name, { menuButton, menu } });

        // Whenever an item was selected in the child menu,
        // this MenuList should also hide its own overlay.
        menu->forwardEmittedEvent(this, "overlayShouldClose");

        // Calculate positioning and size menu items
        _recalculateMenuItemBounds();
    }

    void MenuList::addMenuItem(const MenuItem& item) {
        auto menuItemButton = MakeRef<Button>();
        menuItemButton->cornerRadius = 0;
        menuItemButton->text = item;
        menuItemButton->color = color;
        menuItemButton->backgroundColor = backgroundColor;
        menuItemButton->on("clicked", [this, item](auto e) {
            // Indicate to this MenuList and its potential
            // parent MenuLists that the overlay should close.
            fireEvent("overlayShouldClose", Event::empty);

            // When the menu item is selected,
            // the MenuList should fire an event
            // indicating that an item was selected.
            fireEvent("itemSelected", MakeRef<Event>(eventDataMap_t{
                { "item", item },
                { "index", indexOf(item) }
            }));
        });
        d_contentPanel->addChild(menuItemButton);

        // Add the menu item to the list
        d_menuItems.push_back({ item, { menuItemButton, item } });

        // Calculate positioning and size menu items
        _recalculateMenuItemBounds();
    }

    bool MenuList::removeItem(const std::string& name) {
        bool status = false;

        for (auto it = d_menuItems.begin(); it != d_menuItems.end(); ++it) {
            auto menuItemName   = it->first;
            auto menuItem       = it->second;

            if (name == menuItemName) {
                // Get the button representing the menu item
                auto& menuItemButton = menuItem.first;

                // Remove the click handler from it
                menuItemButton->off("clicked");

                // Remove the item entry from the item list
                d_menuItems.erase(it);

                // Remove the button from the overlay panel
                d_contentPanel->removeChild(menuItemButton->getID());

                // Stop looping
                status = true;
                break;
            }
        }

        // Recalculate item positions and overlay size
        if (status) {
            _recalculateMenuItemBounds();
        }

        return status;
    }

    void MenuList::removeAllItems() {
        while (d_menuItems.size()) {
            // Get the name of the first item
            auto& firstItemName = d_menuItems.at(0).first;

            // Remove it from the list
            removeItem(firstItemName);
        }

        // Recalculate overlay size
        // since there are no more items.
        _recalculateMenuItemBounds();
    }

    void MenuList::setActivatorWidget(BaseWidget* widget) {
        d_overlay->setActivatorWidget(widget);
    }

    size_t MenuList::indexOf(const std::string& name) {
        for (size_t i = 0; i < d_menuItems.size(); ++i) {
            if (d_menuItems.at(i).first == name) {
                return i;
            }
        }

        return -1;
    }

    std::string MenuList::getItemName(size_t index) {
        CORE_ASSERT((d_menuItems.size() && index < d_menuItems.size()), "Item index out of bounds");

        return d_menuItems.at(index).first;
    }
} // namespace mc
