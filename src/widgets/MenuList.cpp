#include "MenuList.h"

namespace mc {
    MenuList::MenuList() {
        _setupProperties();
    }

    MenuList::MenuList(const std::string& name) : name(name) {
        _setupProperties();
    }

    void MenuList::spawn(const Point& anchor) {
        if (!d_overlay->getActivatorWidget()) {
            d_overlay->setAnchor(anchor);
            d_overlay->show();
        }
    }

    void MenuList::_setupProperties() {
        // Create the content panel
        d_contentPanel = MakeRef<StackPanel>();
        d_contentPanel->cornerRadius = 0;

        // Create the overlay
        d_overlay = MakeRef<Overlay>();
        d_overlay->spawnDirection = spawnDirection;
        d_overlay->setContent(d_contentPanel);

        // MenuList-specific properties
        handleWidgetVisiblePropertyChange(name);

        handleWidgetVisiblePropertyChange(borderColor);
        borderColor.forwardAssignment(&d_contentPanel->backgroundColor);
        borderColor = Color::white;

        handleWidgetVisiblePropertyChange(spawnDirection);
        spawnDirection.on("propertyChanged", [this](Shared<Event> e) {
            d_overlay->spawnDirection = spawnDirection;
            });
        spawnDirection = OverflowDirection::Right;

        handleWidgetVisiblePropertyChange(borderThickness);
        borderThickness.on("propertyChanged", [this](Shared<Event> e) {
            _recalculateGeometry();
            });
        borderThickness = 1;

        itemSize = 12;
        handleWidgetVisiblePropertyChange(itemSize);
        itemSize.on("propertyChanged", [this](Shared<Event> e) {
            _recalculateGeometry();
            });

        itemBackgroundColor = Color::darkGray;
        handleWidgetVisiblePropertyChange(itemBackgroundColor);
        itemBackgroundColor.on("propertyChanged", [this](Shared<Event> e) {
            _recalculateStyles();
            });

        itemTextColor = Color::white;
        handleWidgetVisiblePropertyChange(itemTextColor);
        itemTextColor.on("propertyChanged", [this](Shared<Event> e) {
            _recalculateStyles();
            });

        // Allow MenuList specific events
        appendAllowedEvent("itemSelected");
        appendAllowedEvent("overlayShouldClose");

        // Hide the overlay when needed
        on("overlayShouldClose", [this](auto e) {
            d_overlay->hide();
            });
    }

    void MenuList::_recalculateGeometry() {
        Size desiredOverlaySize = Size(0, 0);
        uint64_t itemIndex = 0;

        for (auto& [name, menuItem] : d_menuItems) {
            auto& menuButton = menuItem.first;

            // Update button's margins
            menuButton->marginTop = 0;
            menuButton->marginBottom = 0;
            menuButton->marginLeft = borderThickness;
            menuButton->marginRight = borderThickness;

            if (itemIndex == 0) {
                menuButton->marginTop = borderThickness;
            } else if (itemIndex == d_menuItems.size() - 1) {
                menuButton->marginBottom = borderThickness;
            }

            // Update button's font size
            menuButton->label->fontSize = itemSize;

            // Measure each button's desired size and use it
            // to calculate the overall overlay width and height.
            menuButton->measure();
            auto menuButtonSize = menuButton->getDesiredSizeWithMargins();

            if (menuButtonSize.width > desiredOverlaySize.width) {
                desiredOverlaySize.width = menuButtonSize.width;
            }

            desiredOverlaySize.height += menuButtonSize.height;

            // Increment the item index
            ++itemIndex;
        }

        if (fixedWidth != NOT_SET) {
            desiredOverlaySize.width = fixedWidth;
        }

        if (fixedHeight != NOT_SET) {
            desiredOverlaySize.height = fixedHeight;
        }

        // Resize the overlay to fit the content
        d_overlay->setSize(desiredOverlaySize);
    }

    void MenuList::_recalculateStyles() {
        for (auto& [name, menuItem] : d_menuItems) {
            auto& menuButton = menuItem.first;

            // Update button's style
            menuButton->backgroundColor = itemBackgroundColor;
            menuButton->label->color = itemTextColor;
        }
    }

    Size MenuList::_measureSize() {
        _recalculateGeometry();

        return d_overlay->getSize();
    }

    void MenuList::_onSetComputedSize(const Size& size) {
        for (auto& [name, menuItem] : d_menuItems) {
            auto& menuButton = menuItem.first;
            menuButton->fixedWidth = size.width;
        }

        d_overlay->setSize(size);
    }

    void MenuList::addSubMenu(Shared<MenuList> menu) {
        auto menuButton = MakeRef<Button>();
        menuButton->cornerRadius = 0;
        menuButton->label->text = menu->name;
        menuButton->label->fontSize = itemSize;
        menuButton->label->color = itemTextColor;
        menuButton->borderThickness = 0;
        menuButton->horizontalAlignment = HAFill;
        menuButton->on("hoveredOff", [this, menuButton](Shared<Event> e) {
            auto mme = std::static_pointer_cast<MouseMovedEvent>(e);

            // Track when the mouse is hovered over a different menu item
            // so that any existing submenu's can close their overlay windows.
            auto mousePos = mme->getLocation();
            auto menuButtonLocation = menuButton->getPositionInWindow();

            // Offset the mouse position to be relative to the menu button
            int32_t localMousePos = mousePos.y - menuButtonLocation.y;
            int32_t menuItemHeight = static_cast<int32_t>(menuButton->getComputedSize().height);

            if (localMousePos >= menuItemHeight || localMousePos <= 0) {
                d_overlay->closeChildOverlays();
            }
            });
        d_contentPanel->addChild(menuButton);

        // Set the button as menu list activator
        menu->setActivatorWidget(menuButton.get(), OnHover);

        // Set the menu's overlay as the child overlay
        d_overlay->addChildOverlay(menu->d_overlay);

        // Add the menu to the list
        d_menuItems.push_back({ menu->name, { menuButton, menu } });

        // Whenever an item was selected in the child menu,
        // this MenuList should also hide its own overlay.
        menu->forwardEmittedEvent(this, "overlayShouldClose");

        // Recalculate the overlay's size to fit content
        _recalculateGeometry();
    }

    void MenuList::addMenuItem(const MenuItem& item) {
        auto menuItemButton = MakeRef<Button>();
        menuItemButton->cornerRadius = 0;
        menuItemButton->label->text = item;
        menuItemButton->label->fontSize = itemSize;
        menuItemButton->label->color = itemTextColor;
        menuItemButton->borderThickness = 0;
        menuItemButton->horizontalAlignment = HAFill;
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

        // Recalculate the overlay's size to fit content
        _recalculateGeometry();
    }

    bool MenuList::removeItem(const std::string& name) {
        // Recalculate the overlay's size to fit content
        _recalculateGeometry();

        return false;
    }

    void MenuList::removeAllItems() {
        while (d_menuItems.size()) {
            // Get the name of the first item
            auto& firstItemName = d_menuItems.at(0).first;

            // Remove it from the list
            removeItem(firstItemName);
        }
    }

    void MenuList::setActivatorWidget(BaseWidget* widget, OverlayActivationType activationType) {
        d_overlay->setActivatorWidget(widget, activationType);
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
