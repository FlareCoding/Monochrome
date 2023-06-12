#pragma once
#include "Button.h"
#include "StackPanel.h"

namespace mc {
class TabView : public BaseContainerWidget {
public:
    TabView();

    // @brief TO-DO
    PropertyObserver<Color> tabColor;

    // @brief TO-DO
    PropertyObserver<Color> tabActiveColor;

    // @brief TO-DO
    PropertyObserver<Color> tabTextColor;

    // @brief TO-DO
    PropertyObserver<Color> tabBorderColor;

    // @brief TO-DO
    PropertyObserver<std::string> tabFont;

    // @brief TO-DO
    PropertyObserver<uint32_t> tabFontSize;

    void addTab(const std::string& name, Shared<BaseWidget> tab);
    void openTab(const std::string& name);
    void closeTab(const std::string& name);
    bool isValidTab(const std::string& name);

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

    TabElems _createTabElements(const std::string& tabName);
    TabElems _findTabElemsByTabName(const std::string& name);
    void _removeTabElemsByTabName(const std::string& name);
};
} // namespace mc
