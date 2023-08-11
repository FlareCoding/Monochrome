#pragma once
#include "Button.h"
#include "StackPanel.h"

namespace mc {
class Expander : public BaseWidget {
public:
    Expander();
    explicit Expander(const std::string& title);

    std::string getWidgetName() const override { return "Expander"; }

    // Flag responsible for keeping track if the content is expanded or collapsed
    PropertyObserver<bool> expanded;

    // Buttons responsible for collapsing/expanding the content
    Shared<Button> header;

    // Sets the content that can be expanded/collapsed
    void setContent(Shared<BaseWidget> content);

    // Returns the main content of the expander group
    inline Shared<BaseWidget> getContent() const { return d_content; }

protected:
    Size _measureSize() override;
    void _onSetComputedSize(const Size& size) override;

private:
    void _createVisuals();
    void _setupProperties();

private:
    void _setExpandedState(bool expandedState);

    Shared<StackPanel> d_container;
    Shared<BaseWidget> d_content = nullptr;

    const char d_downArrowUtf8Prefix[6] = {
        static_cast<char>(0x20),
        static_cast<char>(0xe2),
        static_cast<char>(0x96),
        static_cast<char>(0xbc),
        static_cast<char>(0x20),
        0
    };

    const char d_rightArrowUtf8Prefix[6] = {
        static_cast<char>(0x20),
        static_cast<char>(0xe2),
        static_cast<char>(0x96),
        static_cast<char>(0xba),
        static_cast<char>(0x20),
        0
    };
};
} // namespace mc
