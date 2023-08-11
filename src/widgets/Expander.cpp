#include "Expander.h"

namespace mc {
    Expander::Expander() {
        _createVisuals();
        _setupProperties();
    }

    Expander::Expander(const std::string& title) : Expander() {
        header->label->text = title;
    }

    Size Expander::_measureSize() {
        for (auto& child : d_container->getChildren()) {
            auto size = child->getDesiredSize();
        }
        return d_container->getDesiredSize();
    }

    void Expander::_onSetComputedSize(const Size& size) {
        auto desiredHeaderSize = header->getDesiredSizeWithMargins();
        auto finalHeaderSize = Size(size.width, desiredHeaderSize.height);

        header->setComputedSize(finalHeaderSize);
        d_container->setComputedSize(size);
    }

    void Expander::_createVisuals() {
        header = MakeRef<Button>();
        header->label->text = "Expanding Group";
        header->secondaryLeftText = d_rightArrowUtf8Prefix;

        d_container = MakeRef<StackPanel>();
        d_container->backgroundColor = Color::transparent;
        d_container->addChild(header);

        _addChild(d_container);
    }

    void Expander::_setupProperties() {
        header->on("clicked", [this](auto e) {
            expanded = !expanded.get();
        });

        expanded = false;
        expanded.on("propertyChanged", [this](auto e) {
            _setExpandedState(expanded.get());
        });
    }

    void Expander::_setExpandedState(bool expandedState) {
        header->secondaryLeftText =
            expandedState ? d_downArrowUtf8Prefix : d_rightArrowUtf8Prefix;

        if (!d_content) {
            return;
        }

        d_content->visible = expandedState;
    }

    void Expander::setContent(Shared<BaseWidget> content) {
        // Remove any previous content
        if (d_content) {
            d_container->removeChild(d_content);
        }

        // Set the new content
        d_content = content;

        if (d_content) {
            d_container->addChild(d_content);
            d_content->visible = expanded.get();
        }
    }
} // namespace mc
