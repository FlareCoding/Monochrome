#include "Renderer.h"
#include <core/InternalFlags.h>
#include <application/AppManager.h>

namespace mc {
    std::pair<Position, Size> Renderer::getWidgetTruePositionAndSize(
        BaseWidget* widget,
        Position& parentPositionOffset
    ) {
        // Calculating frame dimensions of the widget
        int32_t xPos = parentPositionOffset.x + widget->position->x + widget->marginLeft;
        int32_t yPos = parentPositionOffset.y + widget->position->y + widget->marginTop;

        uint32_t width = widget->size->width - (widget->marginLeft + widget->marginRight);
        uint32_t height = widget->size->height - (widget->marginBottom + widget->marginTop);

        return {
            Position(xPos, yPos),
            Size(width, height)
        };
    }

    std::pair<Position, Size> Renderer::getWidgetTruePositionAndSize(
        const Shared<BaseWidget>& widget,
        Position& parentPositionOffset
    ) {
        return getWidgetTruePositionAndSize(widget.get(), parentPositionOffset);
    }

    void Renderer::renderScene(
        Color& backgroundColor,
        WidgetHostController& widgetHostController,
        Shared<RenderTarget>& renderTarget
    ) {
        // Render the background color layer
        renderTarget->clearScreen(
            backgroundColor.r,
            backgroundColor.g,
            backgroundColor.b,
            backgroundColor.a
        );

        std::vector<Shared<BaseWidget>> widgetsList(
            widgetHostController.getSceneWidgetList()
        );
        Position originAnchor = { 0, 0 };

        for (auto& widget : widgetsList) {
            renderWidget(renderTarget, widget, originAnchor);
        }
    }

    void Renderer::renderWidget(
        Shared<RenderTarget>& renderTarget,
        const Shared<BaseWidget>& widget,
        Position& parentPositionOffset
    ) {
        // Before anything else, check if the widget is visible
        if (!widget->visible) {
            return;
        }

        // First create a clipping layer so the contents
        // don't go outside the bounds of the widget.
        renderTarget->pushClipLayer(
            parentPositionOffset.x + widget->position->x,
            parentPositionOffset.y + widget->position->y,
            widget->size->width,
            widget->size->height
        );

        auto widgetType = widget->getType();

        if (widgetType == "panel") {
            renderPanel(
                renderTarget, std::static_pointer_cast<Panel>(widget),
                parentPositionOffset
            );
        } else if (widgetType == "label") {
            renderLabel(
                renderTarget,
                std::static_pointer_cast<Label>(widget),
                parentPositionOffset
            );
        } else if (widgetType == "button") {
            renderButton(
                renderTarget,
                std::static_pointer_cast<Button>(widget),
                parentPositionOffset
            );
        } else if (widgetType == "checkbox") {
            CORE_ASSERT(false, "Rendering 'checkbox' widget is not implemented yet");
        } else if (widgetType == "slider") {
            CORE_ASSERT(false, "Rendering 'slider' widget is not implemented yet");
        } else if (widgetType == "entry") {
            renderEntry(
                renderTarget,
                std::static_pointer_cast<Entry>(widget),
                parentPositionOffset
            );
        } else if (widgetType == "customRenderedWidget") {
            renderCustomRenderedWidget(
                renderTarget,
                std::dynamic_pointer_cast<IRenderable>(widget),
                parentPositionOffset
            );
        }

        if (widget->isContainer()) {
            // Get the list of all children widgets and clone it
            std::vector<Shared<BaseWidget>> children(
                std::static_pointer_cast<BaseContainerWidget>(widget)->getChildren()
            );

            for (auto& child : children) {
                // Adjust combined parent offset to keep child
                // widgets positioned in relation to their parents.
                parentPositionOffset.x += widget->position->x;
                parentPositionOffset.y += widget->position->y;

                renderWidget(renderTarget, child, parentPositionOffset);

                // Restore the combined parent offset value
                parentPositionOffset.x -= widget->position->x;
                parentPositionOffset.y -= widget->position->y;
            }
        }

        // Restore the clipping layer
        renderTarget->popClipLayer();
    }

    void Renderer::renderPanel(
        Shared<RenderTarget>& renderTarget,
        const Shared<Panel>& panel,
        Position& parentPositionOffset
    ) {
        const auto& [position, size] = getWidgetTruePositionAndSize(panel, parentPositionOffset);

        // Drawing the panel's body rectangle
        renderTarget->drawRectangle(
            position.x, position.y,
            size.width, size.height,
            panel->backgroundColor,
            panel->cornerRadius,
            panel->filled,
            panel->stroke
        );
    }

    void Renderer::renderLabel(
        Shared<RenderTarget>& renderTarget,
        const Shared<Label>& label,
        Position& parentPositionOffset
    ) {
        const auto& [position, size] = getWidgetTruePositionAndSize(label, parentPositionOffset);

        // Draw the label's text
        renderTarget->drawText(
            position.x, position.y,
            size.width, size.height,
            label->color,
            label->text,
            label->font,
            label->fontSize,
            label->fontStyle,
            label->alignment,
            label->wordWrapMode
        );
    }

    void Renderer::renderButton(
           Shared<RenderTarget>& renderTarget,
           const Shared<Button>& button,
           Position& parentPositionOffset
    ) {
        const auto& [position, size] = getWidgetTruePositionAndSize(button, parentPositionOffset);

        bool isMouseHovered = getInternalFlag(
            button->getInternalFlags(), InternalWidgetFlag::WidgetHoveredOn);

        bool isMousePressed = getInternalFlag(
            button->getInternalFlags(), InternalWidgetFlag::MouseDownOnWidget);

        // Calculating the button's background color
        Color buttonBodyColor = button->backgroundColor;

        // Calculating color if the mouse is hovered over the widget
        if (isMouseHovered) {
            if (button->onHoverColor.get() == Color::transparent) {
                buttonBodyColor.r -= static_cast<uint32_t>(
                                        static_cast<float>(buttonBodyColor.r) * 0.3f);

                buttonBodyColor.g -= static_cast<uint32_t>(
                                        static_cast<float>(buttonBodyColor.g) * 0.3f);

                buttonBodyColor.b -= static_cast<uint32_t>(
                                        static_cast<float>(buttonBodyColor.b) * 0.3f);
            } else {
                buttonBodyColor = button->onHoverColor;
            }
        }

        // Calculating the color if the mouse is pressed on the button
        if (isMousePressed) {
            if (button->onMousePressColor.get() == Color::transparent) {
                buttonBodyColor.r -= static_cast<uint32_t>(
                                        static_cast<float>(buttonBodyColor.r) * 0.3f);

                buttonBodyColor.g -= static_cast<uint32_t>(
                                        static_cast<float>(buttonBodyColor.g) * 0.3f);

                buttonBodyColor.b -= static_cast<uint32_t>(
                                        static_cast<float>(buttonBodyColor.b) * 0.3f);
            } else {
                buttonBodyColor = button->onMousePressColor;
            }
        }

        // Draw the body of the button
        renderTarget->drawRectangle(
            position.x, position.y,
            size.width, size.height,
            buttonBodyColor,
            button->cornerRadius,
            button->filled,
            button->stroke
        );

        // Draw the button's text inside
        renderTarget->drawText(
            position.x, position.y,
            size.width, size.height,
            button->color,
            button->text,
            button->font,
            button->fontSize,
            button->fontStyle,
            button->alignment,
            button->wordWrapMode
        );

        // Optionally draw the secondary right-aligned text
        if (!button->secondaryRightText->empty()) {
            renderTarget->drawText(
                position.x, position.y,
                size.width, size.height,
                button->color,
                button->secondaryRightText,
                button->font,
                button->fontSize,
                button->fontStyle,
                "right",
                button->wordWrapMode
            );
        }

        // Optionally draw the secondary left-aligned text
        if (!button->secondaryLeftText->empty()) {
            renderTarget->drawText(
                position.x, position.y,
                size.width, size.height,
                button->color,
                button->secondaryLeftText,
                button->font,
                button->fontSize,
                button->fontStyle,
                "left",
                button->wordWrapMode
            );
        }

        // Draw the border on top of the body
        renderTarget->drawRectangle(
            position.x, position.y,
            size.width, size.height,
            button->borderColor,
            button->cornerRadius,
            false,
            button->borderSize
        );
    }

    // void Renderer::renderCheckbox() {

    // }

    // void Renderer::renderSlider() {

    // }

    void Renderer::renderEntry(
        Shared<RenderTarget>& renderTarget,
        const Shared<Entry>& entry,
        Position& parentPositionOffset
    ) {
        constexpr uint32_t entryTextPadding = 6;

        const auto& [position, size] = getWidgetTruePositionAndSize(entry, parentPositionOffset);

        // Draw the body of the entry
        renderTarget->drawRectangle(
            position.x, position.y,
            size.width, size.height,
            entry->backgroundColor,
            entry->cornerRadius,
            entry->filled,
            entry->stroke
        );

        // Determining the text color based
        // on whether the real text is being
        // rendered or the placeholder.
        Color entryTextColor = entry->color;
        if (entry->text->empty()) {
            entryTextColor.r -= static_cast<uint32_t>(
                                    static_cast<float>(entryTextColor.r) * 0.3f);

            entryTextColor.g -= static_cast<uint32_t>(
                                    static_cast<float>(entryTextColor.g) * 0.3f);

            entryTextColor.b -= static_cast<uint32_t>(
                                    static_cast<float>(entryTextColor.b) * 0.3f);

            entryTextColor.a = 120;
        }

        // Push the text region clip
        renderTarget->pushClipLayer(
            position.x + entry->d_entryTextPadding,
            position.y,
            entry->size->width - (entry->d_entryTextPadding * 2),
            entry->size->height
        );

        // Draw the button's text
        if (entry->text->empty()) {
            renderTarget->drawText(
                position.x + entryTextPadding,
                position.y,
                size.width - (entryTextPadding * 2),
                size.height,
                entryTextColor,
                entry->placeholder.get(),
                entry->font,
                entry->fontSize,
                entry->fontStyle,
                "left",
                "none"
            );
        } else {
            renderTarget->drawText(
                position.x + entry->d_textFrame.position.x,
                position.y + entry->d_textFrame.position.y,
                entry->d_textFrame.size.width,
                entry->d_textFrame.size.height,
                entryTextColor,
                entry->text.get(),
                entry->font,
                entry->fontSize,
                entry->fontStyle,
                "left",
                "none"
            );
        }

        // Pop the text region clip
        renderTarget->popClipLayer();

        // Get runtime utility functions
        auto& runtimeFunctions =
            AppManager::getApplicationContext()->getRuntimeUtilityFunctions();

        // Retrieve the function used to calculate
        // minimum required size for the text in pixels.
        auto getTextPixelSizeFn = runtimeFunctions.getTextPixelSizeFn;

        // Calculate and draw the cursor if it's visible
        // and not in the middle of blinking invisibly.
        if (entry->d_cursorBlinkedVisible) {
            // Determine the minimum required width for the
            // text before the cursor position in pixels.
            auto preCursorText = entry->text->substr(0, entry->d_cursorPos);

            auto [textPixelWidth, textPixelHeight] = getTextPixelSizeFn(
                entry->d_textFrame.size.width,
                entry->d_textFrame.size.height,
                preCursorText, entry->font, entry->fontSize, entry->fontStyle,
                "left", "none"
            );

            // Calculate the cursor's y offset from the top of the entry
            auto cursorYOffset =
                (uint32_t)((static_cast<float>(size.height) - textPixelHeight) / 2);

            // Draw the cursor if the entry is focused
            // and if the entry is not in a read-only mode.
            if (entry->isFocused() && !entry->readOnly) {
                renderTarget->drawRectangle(
                    position.x + entry->d_textFrame.position.x + (int32_t)textPixelWidth,
                    position.y + cursorYOffset,
                    2,
                    (uint32_t)textPixelHeight,
                    entry->cursorColor,
                    0,
                    true,
                    0
                );
            }
        }

        // Calculate and draw the highlighted text
        // region if there is a text selection.
        if (entry->hasSelectedText()) {
            uint64_t selectionStartIndex = entry->d_selectionBeganIndex;
            uint64_t selectionEndIndex = entry->d_selectionEndedIndex;

            if (selectionStartIndex > selectionEndIndex) {
                selectionEndIndex = entry->d_selectionBeganIndex;
                selectionStartIndex = entry->d_selectionEndedIndex;
            }

            // Calculate the distance to the
            // beginning of the selected text frame.
            auto [preTextSelectionWidth, _0] = getTextPixelSizeFn(
                entry->d_textFrame.size.width,
                entry->d_textFrame.size.height,
                entry->text->substr(0, selectionStartIndex),
                entry->font, entry->fontSize, entry->fontStyle,
                "left", "none"
            );

            // Calculate the distance to the
            // end of the selected text frame.
            auto [postTextSelectionWidth, _1] = getTextPixelSizeFn(
                entry->d_textFrame.size.width,
                entry->d_textFrame.size.height,
                entry->text->substr(0, selectionEndIndex),
                entry->font, entry->fontSize, entry->fontStyle,
                "left", "none"
            );

            uint32_t selectionWidth = (uint32_t)(postTextSelectionWidth - preTextSelectionWidth);

            // Draw the actual selection rectangle
            renderTarget->drawRectangle(
                position.x + entry->d_textFrame.position.x + (int32_t)preTextSelectionWidth,
                position.y + entry->d_textFrame.position.y,
                selectionWidth,
                entry->d_textFrame.size.height,
                entry->selectionColor,
                0, true, 0
            );
        }

        // Draw the border on top of the body
        // if the entry is not in a read-only mode.
        if (!entry->readOnly) {
            renderTarget->drawRectangle(
                position.x, position.y,
                size.width, size.height,
                entry->isFocused() ? entry->focusedHighlightColor : entry->borderColor,
                entry->cornerRadius,
                false,
                entry->borderSize
            );
        }
    }

    void Renderer::renderCustomRenderedWidget(
        Shared<RenderTarget>& renderTarget,
        const Shared<IRenderable>& renderable,
        Position& parentPositionOffset
    ) {
        const getWidgetBoundsFn_t getWidgetBounds =
            [](BaseWidget* widget, Position& parentPositionOffset) {
                return getWidgetTruePositionAndSize(widget, parentPositionOffset);
            };

        renderable->onRender(renderTarget, parentPositionOffset, getWidgetBounds);
    }
} // namespace mc
