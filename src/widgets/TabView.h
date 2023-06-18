#pragma once
#include "Button.h"
#include "StackPanel.h"

namespace mc {
class TabView : public BaseContainerWidget {
public:
    TabView();

    // Color of each tab button
    PropertyObserver<Color> tabColor;

    // Color of the currently opened tab button
    PropertyObserver<Color> tabActiveColor;

    // Color of the tab button's text
    PropertyObserver<Color> tabTextColor;

    // Color of the border between tab buttons
    PropertyObserver<Color> tabBorderColor;

    // Font of each tab button's text
    PropertyObserver<std::string> tabFont;

    // Font size of each tab button's text
    PropertyObserver<uint32_t> tabFontSize;

    // Creates a button in the tab bar that links to the given widget
    // @param name Name of the tab
    // @param tab Widget to be displayed in the tab
    // @param closable Specifies whether or not you can close the tab via a close button
    void addTab(const std::string& name, Shared<BaseWidget> tab, bool closable = false);

    // Opens the specified tab
    void openTab(const std::string& name);

    // Closes the specified tab
    void closeTab(const std::string& name);

    // Returns true if the tab is in the tab bar
    bool isValidTab(const std::string& name);

    // Returns the name of the currently
    // opened tab, empty string otherwise.
    inline std::string getActiveTab() const { return d_activeTab; }

protected:
    Size _measureSize() override;
    void _onArrangeChildren() override;

private:
    void _createVisuals();
    void _setupProperties();

private:
    std::string d_activeTab;

    std::map<std::string, Shared<BaseWidget>> d_tabViews;

    struct TabElems {
        Shared<StackPanel> tabContainer;
        Shared<Button> tabButton;
        Shared<Button> tabCloseButton;
    };

    std::vector<std::pair<std::string, TabElems>> d_tabButtons;

private:
    void _resetTabElementsProps(bool layoutChange);

    TabElems _createTabElements(const std::string& tabName, bool tabClosable);
    TabElems _findTabElemsByTabName(const std::string& name);
    void _removeTabElemsByTabName(const std::string& name);
};
} // namespace mc
