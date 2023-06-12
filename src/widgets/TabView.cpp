#include "TabView.h"
#include "visuals/RectVisual.h"

namespace mc {
    TabView::TabView() {
        _setupProperties();
    }

    Size TabView::_measureSize() {
        // Measure the tabs
        Position availablePos = Position(0, 0);
        uint32_t tabHeight = 0;
        Size contentSize = Size(0, 0);

        for (auto& [tabName, tabElems] : d_tabButtons) {
            auto tabContainer = tabElems.tabContainer;

            auto desiredSize = tabContainer->getDesiredSizeWithMargins();
            contentSize.width += desiredSize.width;

            tabHeight = desiredSize.height;
        }

        contentSize.height += tabHeight;

        // Measure the active tab's view
        if (d_activeTab.empty()) {
            return contentSize;
        }

        auto activeView = d_tabViews.at(d_activeTab);
        auto desiredActiveViewSize = activeView->getDesiredSizeWithMargins();
        
        contentSize.width = std::max(contentSize.width, desiredActiveViewSize.width);
        contentSize.height += desiredActiveViewSize.height;

        return contentSize;
    }

    void TabView::_onArrangeChildren() {
        // Arrange the tabs
        Position tabPos = Position(0, 0);
        uint32_t tabHeight = 0;
        auto contentSize = getComputedSize();

        for (auto& [tabName, tabElems] : d_tabButtons) {
            auto tabContainer = tabElems.tabContainer;

            auto desiredSize = tabContainer->getDesiredSizeWithMargins();
            tabContainer->setComputedSize(desiredSize);

            tabContainer->position = tabPos;
            tabPos.x += desiredSize.width;

            tabHeight = desiredSize.height;
        }

        // Arrange the active tab's view
        if (d_activeTab.empty()) {
            return;
        }

        auto activeView = d_tabViews.at(d_activeTab);
        auto activeViewSize = Size(contentSize.width, contentSize.height - tabHeight);

        activeView->setComputedSize(activeViewSize);
        activeView->position = Position(0, tabHeight);
    }

    void TabView::_setupProperties() {
        tabColor = Color(40, 47, 48); // default value
        tabActiveColor = Color(28, 71, 94); // default value
        tabTextColor = Color::white; // default value
        tabBorderColor = Color::white; // default value
        tabFont = "Arial"; // default value
        tabFontSize = 12; // default value

        tabColor.on(
            "propertyChanged", [this](auto e) { _resetTabElementsProps(false); });
        tabActiveColor.on(
            "propertyChanged", [this](auto e) { _resetTabElementsProps(false); });
        tabTextColor.on(
            "propertyChanged", [this](auto e) { _resetTabElementsProps(false); });
        tabBorderColor.on(
            "propertyChanged", [this](auto e) { _resetTabElementsProps(false); });
        tabFont.on(
            "propertyChanged", [this](auto e) { _resetTabElementsProps(true); });
        tabFontSize.on(
            "propertyChanged", [this](auto e) { _resetTabElementsProps(true); });

        d_activeTab = "";
    }

    void TabView::addTab(const std::string& name, Shared<BaseWidget> tab) {
        CORE_ASSERT(d_tabViews.find(name) == d_tabViews.end(), "Tab with name '" + name + "' already exists");
        d_tabViews.insert({ name, tab });

        TabElems tabElems = _createTabElements(name);
        d_tabButtons.push_back({name, tabElems });

        addChild(tabElems.tabContainer);

        if (d_activeTab.empty()) {
            openTab(name);
        }
    }

    void TabView::openTab(const std::string& name) {
        // Check if the tab is already opened
        if (d_activeTab == name) {
            return;
        }

        CORE_ASSERT(d_tabViews.find(name) != d_tabViews.end(), "Tab with name '" + name + "' doesn't exist");

        // If there is a currently opened tab
        if (!d_activeTab.empty()) {
            // Get the current active tab's view
            auto currentActiveView = d_tabViews.at(d_activeTab);

            // Get the current active tab button elements
            auto currentTabElems = _findTabElemsByTabName(d_activeTab);

            // Remove the 'active' color from the current tab
            currentTabElems.tabButton->backgroundColor = tabColor;
            currentTabElems.tabCloseButton->backgroundColor = tabColor;

            // Remove the current view
            removeChild(currentActiveView);
        }

        // Get the new active tab's view
        auto newActiveView = d_tabViews.at(name);

        // Get the new active tab button elements
        auto newTabElems = _findTabElemsByTabName(name);

        // Update the active tab name 
        d_activeTab = name;

        newTabElems.tabButton->backgroundColor = tabActiveColor;
        newTabElems.tabCloseButton->backgroundColor = tabActiveColor;

        // Set the new view
        addChild(newActiveView);
    }

    void TabView::closeTab(const std::string& name) {
        CORE_ASSERT(d_tabViews.find(name) != d_tabViews.end(), "Tab with name '" + name + "' doesn't exist");

        // If the tab being closed is the currently opened one,
        // remove the active view and open a different tab if possible.
        if (d_activeTab == name) {
            // Get the current active tab's view
            auto activeView = d_tabViews.at(name);

            // Find the adjacent tab
            if (d_tabViews.size() > 1) {
                for (size_t i = 0; i < d_tabButtons.size(); ++i) {
                    auto& [tabName, _] = d_tabButtons.at(i);

                    if (tabName == name) {
                        std::string newTab = "";

                        // Check if there is a tab opened to the left
                        if (i > 0) {
                            newTab = d_tabButtons.at(i - 1).first;
                        } else {
                            // Get the tab to the right
                            newTab = d_tabButtons.at(i + 1).first;
                        }

                        // Open the new tab
                        openTab(newTab);
                        break;
                    }
                }
            } else {
                // If only one tab is remaining, simply remove the view
                removeChild(activeView);
                d_activeTab = "";
            }
        }

        // Get tab button elements
        auto tabElems = _findTabElemsByTabName(name);

        // Due to one of these widgets having focus, erasing them causes
        // a memory corruption bug. In order to prevent it, each of the
        // tab widgets have to be unfocused before getting removed.
        tabElems.tabButton->unfocus();
        tabElems.tabCloseButton->unfocus();
        tabElems.tabContainer->unfocus();

        // Remove all map references to the tab
        d_tabViews.erase(name);
        _removeTabElemsByTabName(name);

        // Remove the tab button elements
        removeChild(tabElems.tabContainer);
    }

    bool TabView::isValidTab(const std::string& name) {
        return (d_tabViews.find(name) != d_tabViews.end());
    }

    void TabView::_resetTabElementsProps(bool layoutChange) {
        for (auto& [tabName, tabElems] : d_tabButtons) {
            if (tabName == d_activeTab) {
                tabElems.tabButton->backgroundColor = tabActiveColor;
                tabElems.tabCloseButton->backgroundColor = tabActiveColor;
            } else {
                tabElems.tabButton->backgroundColor = tabColor;
                tabElems.tabCloseButton->backgroundColor = tabColor;
            }

            tabElems.tabContainer->backgroundColor = tabBorderColor;
            
            tabElems.tabButton->label->color = tabTextColor;
            tabElems.tabCloseButton->label->color = tabTextColor;

            tabElems.tabButton->label->font = tabFont;
            tabElems.tabCloseButton->label->font = tabFont;

            tabElems.tabButton->label->fontSize = tabFontSize;
            tabElems.tabCloseButton->label->fontSize = tabFontSize;
        }

        if (layoutChange) {
            fireEvent("layoutChanged", Event::empty);
        } else {
            fireEvent("propertyChanged", Event::empty);
        }
    }

    TabView::TabElems TabView::_createTabElements(const std::string& tabName) {
        auto tabContainerPanel = MakeRef<StackPanel>();
        tabContainerPanel->orientation = Horizontal;
        tabContainerPanel->marginRight = 1;
        tabContainerPanel->marginBottom = 1;
        tabContainerPanel->backgroundColor = tabBorderColor;

        auto tabButton = MakeRef<Button>();
        tabButton->label->text = tabName;
        tabButton->label->font = tabFont;
        tabButton->label->fontSize = tabFontSize;
        tabButton->backgroundColor = tabColor;
        tabButton->borderThickness = 1;
        tabButton->cornerRadius = 0;
        tabButton->borderColor = Color::transparent;
        tabButton->on("clicked", [this](Shared<Event> e) {
            auto tabName = static_cast<Button*>(e->target)->label->text;
            openTab(tabName);
        });

        auto tabCloseButton = MakeRef<Button>();
        tabCloseButton->label->horizontalPadding = 12;
        tabCloseButton->label->text = "x";
        tabCloseButton->label->font = tabFont;
        tabCloseButton->label->fontSize = tabFontSize;
        tabCloseButton->backgroundColor = tabColor;
        tabCloseButton->borderThickness = 1;
        tabCloseButton->cornerRadius = 0;
        tabCloseButton->borderColor = Color::transparent;
        tabCloseButton->on("clicked", [this](Shared<Event> e) {
            auto parentContainer = static_cast<BaseContainerWidget*>(e->target->getParent());
            CORE_ASSERT(parentContainer != nullptr,
                "TabView: fatal error retrieving the parent of a tab close button");

            auto tabNameButton = std::static_pointer_cast<Button>(parentContainer->getChild(0));
            auto tabName = tabNameButton->label->text;

            // Close the tab
            closeTab(tabName);
        });

        tabContainerPanel->addChild(tabButton);
        tabContainerPanel->addChild(tabCloseButton);

        TabElems tab;
        tab.tabContainer = tabContainerPanel;
        tab.tabButton = tabButton;
        tab.tabCloseButton = tabCloseButton;

        return tab;
    }

    TabView::TabElems TabView::_findTabElemsByTabName(const std::string& name) {
        for (auto& [tabName, tabElems] : d_tabButtons) {
            if (tabName == name) {
                return tabElems;
            }
        }

        return TabElems();
    }

    void TabView::_removeTabElemsByTabName(const std::string& name) {
        for (auto it = d_tabButtons.begin(); it != d_tabButtons.end(); ++it) {
            auto& [tabName, tabElems] = *it;

            if (tabName == name) {
                d_tabButtons.erase(it);
                break;
            }
        }
    }
} // namespace mc
