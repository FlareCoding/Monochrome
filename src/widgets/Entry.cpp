#include "Entry.h"
#include <events/KeyboardEvents.h>
#include <events/MouseEvents.h>
#include <application/AppManager.h>
#include <utils/Clipboard.h>
#include <cmath>

namespace mc {
    Entry::Entry() {
        appendAllowedEvent("changed");
        appendAllowedEvent("entered");
        _createVisuals();
        _setupProperties();
    }

    Entry::Entry(const std::string& text) : Entry() {
        this->text = text;
    }

    Size Entry::_measureSize() {
        auto textSize = d_textVisual->measureSize();

        return Size(fixedWidth, textSize.height + 8);
    }

    void Entry::_onSetComputedSize(const Size& size) {
        // Calculate the minimum text visual width (within clipping region)
        auto minimumTextVisualWidth = size.width - d_textPadding * 2;

        // Adjust the clipping region according to the size of the entry
        d_textClippingRegionVisual->position = Position(d_textPadding, 0);
        d_textClippingRegionVisual->customWidth = minimumTextVisualWidth;
        d_textClippingRegionVisual->customHeight = size.height;

        // Make sure that the area that displays text is
        // always at least the available size inside the entry.
        if (d_textVisual->customWidth < minimumTextVisualWidth) {
            d_textVisual->customWidth = minimumTextVisualWidth;
        }

        _updateTextFrameSize();
    }

    void Entry::_createVisuals() {
        // Entry's body rect
        d_bodyVisual = MakeRef<RectVisual>();
        cornerRadius.forwardAssignment(&d_bodyVisual->cornerRadius);
        backgroundColor.forwardAssignment(&d_bodyVisual->color);
        addCoreVisualElement(d_bodyVisual);

        // Entry's border
        d_borderVisual = MakeRef<BorderVisual>();
        cornerRadius.forwardAssignment(&d_borderVisual->cornerRadius);
        borderColor.forwardAssignment(&d_borderVisual->color);
        borderThickness.forwardAssignment(&d_borderVisual->thickness);
        addCoreVisualElement(d_borderVisual);

        // Selection highlight visual
        d_selectionHighlightVisual = MakeRef<RectVisual>();
        d_selectionHighlightVisual->customWidth = 1;
        d_selectionHighlightVisual->customHeight = 1;
        selectionHighlightColor.forwardAssignment(&d_selectionHighlightVisual->color);
        addCoreVisualElement(d_selectionHighlightVisual);

        // Text clipping region
        d_textClippingRegionVisual = MakeRef<ClipRegionVisual>();
        addCoreVisualElement(d_textClippingRegionVisual);

        // Text visual
        d_textVisual = MakeRef<TextVisual>();
        d_textVisual->position->x = d_textPadding;
        text.forwardAssignment(&d_textVisual->text);
        textColor.forwardAssignment(&d_textVisual->color);
        font.forwardAssignment(&d_textVisual->font);
        fontSize.forwardAssignment(&d_textVisual->fontSize);
        fontStyle.forwardAssignment(&d_textVisual->fontStyle);
        d_textVisual->alignment = "left";
        d_textVisual->wordWrapMode = "none";
        addCoreVisualElement(d_textVisual);

        // Cursor visual
        d_cursorVisual = MakeRef<RectVisual>();
        d_cursorVisual->customWidth = 2;
        d_cursorVisual->visible = false;
        d_cursorVisual->cornerRadius = 0;
        cursorColor.forwardAssignment(&d_cursorVisual->color);
        addCoreVisualElement(d_cursorVisual);
    }

    void Entry::_setupProperties() {
        font = "Arial";
        fontSize = 14;
        fontStyle = "normal";

        on("gainedFocus",   &Entry::_gainedFocus, this);
        on("lostFocus",     &Entry::_lostFocus, this);
        on("keyDown",       &Entry::_onKeyDown, this);
        on("mouseDown",     &Entry::_onMouseDown, this);
        on("mouseUp",       &Entry::_onMouseUp, this);
        on("mouseMoved",    &Entry::_onMouseMoved, this);

        text.on("propertyChanged", [this](Shared<Event> e) {
            _onTextAssigned();
        });
        handleWidgetVisiblePropertyChange(text);

        placeholder = "Type here...";
        handleWidgetVisiblePropertyChange(placeholder);

        handleWidgetLayoutPropertyChange(font);
        handleWidgetVisiblePropertyChange(font);

        handleWidgetLayoutPropertyChange(fontSize);
        handleWidgetVisiblePropertyChange(fontSize);

        borderColor = Color::transparent;
        handleWidgetVisiblePropertyChange(borderColor);

        focusedBorderColor = Color::cyan;
        handleWidgetVisiblePropertyChange(focusedBorderColor);

        backgroundColor = BaseWidget::getUniversalBackgroundColor();
        handleWidgetVisiblePropertyChange(backgroundColor);

        textColor = Color::white;
        handleWidgetVisiblePropertyChange(textColor);

        cornerRadius = 6;
        handleWidgetVisiblePropertyChange(cornerRadius);

        borderThickness = 2;
        handleWidgetVisiblePropertyChange(borderThickness);

        readOnly = false;
        handleWidgetVisiblePropertyChange(readOnly);

        cursorColor = Color::white;
        handleWidgetVisiblePropertyChange(cursorColor);

        selectionHighlightColor = Color(66, 135, 245);
        handleWidgetVisiblePropertyChange(selectionHighlightColor);

        handleWidgetVisiblePropertyChange(font);
        handleWidgetVisiblePropertyChange(fontSize);
        handleWidgetVisiblePropertyChange(fontStyle);

        // The following properties correspond
        // to default values in the text visual.
        cursorType = CursorType::IBeam;
        fixedWidth = 160;
        text = "";

        markMouseDraggable();
    }

    void Entry::clear() {
        text = "";
        d_textVisual->text = "";

        clearSelection();
    }

    bool Entry::isEmpty() {
        return !text->size();
    }

    bool Entry::hasSelectedText() {
        return d_selectionBeganIndex != d_selectionEndedIndex;
    }

    std::string Entry::getSelectedText() {
        uint64_t selectionLength = std::llabs(
            static_cast<int64_t>(d_selectionEndedIndex) -
            static_cast<int64_t>(d_selectionBeganIndex));

        if (d_selectionEndedIndex > d_selectionBeganIndex) {
            return text->substr(d_selectionBeganIndex, selectionLength);
        } else {
            return text->substr(d_selectionEndedIndex, selectionLength);
        }
    }

    void Entry::clearSelection() {
        d_selectionBeganIndex = d_cursorPos;
        d_selectionEndedIndex = d_cursorPos;

        _calculateSelectionSizeAndOffset();
    }

    void Entry::_gainedFocus(Shared<Event> e) {
        if (readOnly) {
            return;
        }

        d_borderVisual->color = focusedBorderColor;
        d_cursorVisual->visible = true;

        requestRepaint();
    }

    void Entry::_lostFocus(Shared<Event> e) {
        d_borderVisual->color = borderColor;
        d_cursorVisual->visible = false;

        clearSelection();
        requestRepaint();
    }

    void Entry::_onKeyDown(Shared<Event> e) {
        if (readOnly) {
            return;
        }

        auto kde = std::static_pointer_cast<KeyDownEvent>(e);

        KeyCode keycode = kde->getKeyCode();
        bool ctrlPressed = kde->isCtrlPressed();
        bool shiftPressed = kde->isShiftPressed();

        switch (keycode) {
        case KeyCode::KEY_BACKSPACE: {
            _onBackspace(ctrlPressed);
            break;
        }
        case KeyCode::KEY_TAB: {
            fireEvent("requestedFocusLoss", Event::empty);
            break;
        }
        case KeyCode::KEY_LEFT: {
            _onLeftArrowKey(ctrlPressed, shiftPressed);
            break;
        }
        case KeyCode::KEY_RIGHT: {
            _onRightArrowKey(ctrlPressed, shiftPressed);
            break;
        }
        case KeyCode::KEY_ENTER: {
            fireEvent("entered", {
                { "text", text.get() }
            }, this);
            break;
        }
        default: {
            char inputChar = kde->getChar();
            if (inputChar) {
                _onCharacterAppended(inputChar, ctrlPressed);
            }
            break;
        }
        }
    }

    void Entry::_onMouseDown(Shared<Event> e) {
        d_mousePressed = true;
        d_positionInWindow = getPositionInWindow();

        if (text->empty()) {
            return;
        }

        auto mde = std::static_pointer_cast<MouseButtonEvent>(e);
        auto clickPosX = mde->getLocation().x;

        d_cursorPos = _getTextIndexFromMousePosition(clickPosX);

        clearSelection();
        _updateTextFrameSize();

        requestRepaint();
    }

    void Entry::_onMouseUp(Shared<Event> e) {
        d_mousePressed = false;
    }

    void Entry::_onMouseMoved(Shared<Event> e) {
        // Only react to mouse "dragging"
        if (!d_mousePressed) {
            return;
        }

        auto mde = std::static_pointer_cast<MouseButtonEvent>(e);
        auto mousePosX = mde->getLocation().x;

        // Update cursor and selection indices
        d_cursorPos = _getTextIndexFromMousePosition(mousePosX);
        d_selectionEndedIndex = d_cursorPos;

        // Update visuals and request a redraw
        _updateTextFrameSize();

        requestRepaint();
    }

    void Entry::_onTextAssigned() {
        // Validating assigned text
        if (d_entryInputValidator) {
            for (uint64_t i = 0; i < text->size(); ++i) {
                d_entryInputValidator(text->at(i));
            }
        }

        d_cursorPos = text->size();
        clearSelection();

        _updateTextFrameSize();

        // Clear the undo/redo history
        d_history = std::stack<std::string>();
        d_undoHistory = std::stack<std::string>();
    }

    bool Entry::_handleShortcut(char cmdChar) {
        switch (cmdChar) {
        case 'c': {
            // Copy operation
            if (hasSelectedText()) {
                auto selectedText = getSelectedText();
                utils::Clipboard::saveToClipboard(selectedText);
            }

            return true;
        }
        case 'v': {
            // Paste operation

            // Do nothing if the entry
            // is in a read-only mode.
            if (readOnly) {
                break;
            }

            // Check if there is currently a selection,
            // and if so, replace it with pasted contents
            if (hasSelectedText()) {
                _eraseSelectedText();
                clearSelection();
            }

            auto pastedText = utils::Clipboard::getClipboardText();

            // Validate pasted text
            if (d_entryInputValidator) {
                for (uint64_t i = 0; i < pastedText.size(); ++i) {
                    if (!d_entryInputValidator(pastedText.at(i))) {
                        // Return true in order to indicate
                        // that the shortcut was handled properly.
                        return true;
                    }
                }
            }

            text->insert(d_cursorPos, pastedText);
            d_textVisual->text->insert(d_cursorPos, pastedText);

            d_cursorPos += pastedText.size();
            _handleEntryTextChanged();

            return true;
        }
        case 'x': {
            // Do nothing if the entry
            // is in a read-only mode.
            if (readOnly) {
                break;
            }

            // Cut operation
            if (hasSelectedText()) {
                auto selectedText = getSelectedText();
                utils::Clipboard::saveToClipboard(selectedText);

                _eraseSelectedText();
                clearSelection();
                _handleEntryTextChanged();
            }

            return true;
        }
        case 'z': {
            // Do nothing if the entry
            // is in a read-only mode.
            if (readOnly) {
                break;
            }
            // Undo operation
            if (d_history.size() >= 2) {
                // Update the undo history
                d_undoHistory.push(text.get());

                // Pop the latest change
                d_history.pop();

                // Set the text to the last changed text
                text->assign(d_history.top());

                // Sync the visual's text
                d_textVisual->text->assign(d_history.top());

                // Sanitize the cursor position
                d_cursorPos = text->size();
            }

            return true;
        }
        case 'y': {
            // Do nothing if the entry
            // is in a read-only mode.
            if (readOnly) {
                break;
            }

            // Redo operation
            if (!d_undoHistory.empty()) {
                // Update the text
                text->assign(d_undoHistory.top());

                // Sync the visual's text
                d_textVisual->text->assign(d_undoHistory.top());

                // Pop the last undo operation
                d_undoHistory.pop();

                // Sanitize the cursor position
                d_cursorPos = text->size();
            }

            return true;
        }
        default: break;
        }

        return false;
    }

    void Entry::_onCharacterAppended(char inputChar, bool ctrlPressed) {
        // Check for any potential shortcuts
        // such as Ctrl+C, Ctrl+V, etc.
        if (ctrlPressed) {
            bool shortcutHandled = _handleShortcut(inputChar);

            if (shortcutHandled) {
                _updateTextFrameSize();

                requestRepaint();
                return;
            }
        }

        // Do not handle any other key presses
        // if the entry is in a read-only state.
        if (readOnly) {
            return;
        }

        // Validate the inputted character
        if (d_entryInputValidator) {
            if (!d_entryInputValidator(inputChar)) {
                return;
            }
        }

        // Whenever a character is typed,
        // selection should be erased and cleared.
        if (hasSelectedText()) {
            _eraseSelectedText();
        }

        // If the TextVisual's text contains a placeholder, remove it
        if (text->empty()) {
            d_textVisual->text = "";
        }

        // Insert the character into the text string
        text->insert(text->begin() + d_cursorPos, inputChar);

        // Sync the TextVisual's text
        d_textVisual->text->insert(d_textVisual->text->begin() + d_cursorPos, inputChar);

        // Advance the cursor
        ++d_cursorPos;

        _handleEntryTextChanged();
        _updateTextFrameSize();

        requestRepaint();
    }

    void Entry::_onBackspace(bool ctrlPressed) {
        // Check if the entry has any text selected,
        // because if so, it doesn't matter if CTRL is
        // pressed or not, only the selection should be deleted.
        if (hasSelectedText()) {
            // Deleting selected text string
            _eraseSelectedText();

            // Reset the selection
            clearSelection();
        } else {
            // Do nothing if the cursor is
            // already at the beginning of the text.
            if (d_cursorPos == 0) {
                return;
            }

            if (ctrlPressed) {
                auto preCursorText = text->substr(0, d_cursorPos);
                auto postCursorText = text->substr(d_cursorPos);

                const auto pos = preCursorText.find_last_of(" \t\n");
                if (pos == std::string::npos) {
                    text->assign(postCursorText);
                    d_textVisual->text->assign(postCursorText);

                    d_cursorPos = 0;
                } else {
                    text->assign(text->substr(0, pos) + postCursorText);
                    d_textVisual->text->assign(text->substr(0, pos) + postCursorText);

                    d_cursorPos = pos;
                }
            } else {
                text->erase(d_cursorPos - 1, 1);
                d_textVisual->text->erase(d_cursorPos - 1, 1);

                --d_cursorPos;
            }
        }

        _handleEntryTextChanged();
        _updateTextFrameSize();

        requestRepaint();
    }

    void Entry::_onLeftArrowKey(bool ctrlPressed, bool shiftPressed) {
        if (d_cursorPos == 0) {
            return;
        }

        if (ctrlPressed) {
            auto preCursorText = text->substr(0, d_cursorPos);

            const auto pos = preCursorText.find_last_of(" \t\n");
            if (pos == std::string::npos) {
                d_cursorPos = 0;
            } else {
                d_cursorPos = pos;
            }
        } else {
            --d_cursorPos;
        }

        // Check if shift is pressed, and if
        // so, update the selection end index.
        if (shiftPressed) {
            d_selectionEndedIndex = d_cursorPos;
        } else {
            clearSelection();
        }

        _updateTextFrameSize();

        requestRepaint();
    }

    void Entry::_onRightArrowKey(bool ctrlPressed, bool shiftPressed) {
        if (d_cursorPos >= text->size()) {
            return;
        }

        if (ctrlPressed) {
            auto postCursorText = text->substr(d_cursorPos);

            auto pos = postCursorText.find_first_of(" \t\n");
            if (pos == 0) {
                pos = postCursorText.find_first_not_of(" \t\n");
            }

            if (pos == std::string::npos) {
                d_cursorPos = text->size();
            } else {
                d_cursorPos = d_cursorPos + pos;
            }
        } else {
            ++d_cursorPos;
        }

        // Check if shift is pressed, and if
        // so, update the selection end index.
        if (shiftPressed) {
            d_selectionEndedIndex = d_cursorPos;
        } else {
            clearSelection();
        }

        _updateTextFrameSize();

        requestRepaint();
    }

    void Entry::_eraseSelectedText() {
        uint64_t selectionLength = std::llabs(
            static_cast<int64_t>(d_selectionEndedIndex) -
            static_cast<int64_t>(d_selectionBeganIndex));

        uint64_t selectionStartIndex = d_selectionBeganIndex;

        // Find the index that indicates the start of the selection
        if (selectionStartIndex > d_selectionEndedIndex) {
            selectionStartIndex = d_selectionEndedIndex;
        }

        // Erase the selected text
        text->erase(selectionStartIndex, selectionLength);
        d_textVisual->text->erase(selectionStartIndex, selectionLength);

        // Update the cursor position
        d_cursorPos = selectionStartIndex;

        // Clear the selection
        clearSelection();
    }

    void Entry::_handleEntryTextChanged() {
        // Update the text history
        d_history.push(text.get());

        fireEvent("changed", {
            { "text", text.get() }
        }, this);
    }

    void Entry::_updateTextFrameSize() {
        auto computedSize = getComputedSize();

        // Default the visual color to text color before placeholder check
        d_textVisual->color = textColor;

        // Determine if actual text or placeholder should be displayed
        auto displayedText = text.get();

        if (isEmpty()) {
            d_textVisual->text = placeholder.get();
            displayedText = placeholder.get();

            // Slightly dim the color of the text
            // if a placeholder is being used.
            d_textVisual->color = Color::lightGray;
        }

        // Get runtime utility functions
        auto& runtimeFunctions =
            AppManager::getApplicationContext()->getRuntimeUtilityFunctions();

        // Retrieve the function used to calculate
        // minimum required size for the text in pixels.
        auto getTextPixelSizeFn = runtimeFunctions.getTextPixelSizeFn;

        // Determine the minimum required width for the text in pixels
        auto [textPixelWidth, textPixelHeight] = getTextPixelSizeFn(
            std::numeric_limits<uint64_t>::max(), computedSize.height,
            displayedText, font, fontSize, "normal",
            "left", "none"
        );

        d_textVisual->customWidth = static_cast<uint64_t>(textPixelWidth);

        // Check if text frame of reference should be,
        // moved left ultimately scrolling the text forward.
        if (d_cursorPos == text->size()) {
            int32_t diff = (d_textVisual->position->x + d_textVisual->customWidth) -
                            (computedSize.width - d_textPadding);

            if (diff > 0) {
                d_textVisual->position->x -= diff;
            }
        }

        // Additionally check if the text fully fits into the entry,
        // then it has to be aligned with the left-most clip region boundary.
        if (textPixelWidth <= (computedSize.width - (d_textPadding * 2))) {
            d_textVisual->position->x = d_textPadding;
        }

        // In case of text deletion from the back, the text frame
        // can remain shifted over tot eh left, so it needs to be
        // aligned with either the left or the right clip boundaries.
        int32_t rightDiff =
                static_cast<int32_t>(d_textVisual->position->x + d_textVisual->customWidth) -
                static_cast<int64_t>(computedSize.width - d_textPadding);

        int32_t leftDiff = static_cast<int32_t>(d_textPadding) - d_textVisual->position->x;

        if (leftDiff > 0 && rightDiff < 0) {
            int32_t shiftDiff = std::min(std::abs(rightDiff), std::abs(leftDiff));

            d_textVisual->position->x += shiftDiff;
        }

        // Determine if the text needs to be
        // scrolled when the cursor is too far behind.
        auto [preCursorTextWidth, _0] = getTextPixelSizeFn(
            std::numeric_limits<uint64_t>::max(), computedSize.height,
            text->substr(0, d_cursorPos), font, fontSize, fontStyle,
            "left", "none"
        );

        int32_t preCursorDiff = (d_textPadding -
                        (d_textVisual->position->x + static_cast<int32_t>(preCursorTextWidth)));

        if (preCursorDiff > 0) {
            d_textVisual->position->x += preCursorDiff;
        }

        // If the text is added to the middle of the string,
        // the text needs to be scrolled back only after the
        // cursor reaches the right boundary of the clip region.
        int32_t cursorPoint =
                d_textVisual->position->x + static_cast<int32_t>(preCursorTextWidth) +
                d_cursorVisual->customWidth;

        int32_t rightBoundary = computedSize.width - d_textPadding;
        int32_t postCursorDiff = cursorPoint - rightBoundary;

        if (postCursorDiff > 0) {
            d_textVisual->position->x -= postCursorDiff;
        }

        // Update the cursor visual
        _calculateCursorSizeAndOffset();

        // Update the selection visual
        _calculateSelectionSizeAndOffset();
    }

    void Entry::_calculateCursorSizeAndOffset() {
        auto computedSize = getComputedSize();

        // Get runtime utility functions
        auto& runtimeFunctions =
            AppManager::getApplicationContext()->getRuntimeUtilityFunctions();

        // Retrieve the function used to calculate
        // minimum required size for the text in pixels.
        auto getTextPixelSizeFn = runtimeFunctions.getTextPixelSizeFn;

        // Calculate the length of the text before the cursor
        auto preCursorText = text->substr(0, d_cursorPos);

        auto [preCursorTextWidth, textHeight] = getTextPixelSizeFn(
            std::numeric_limits<uint64_t>::max(), computedSize.height,
            preCursorText, font, fontSize, "normal",
            "left", "none"
        );

        // Calculate the cursor's y offset from the top of the entry
        auto cursorOffsetY = static_cast<uint32_t>(
            (static_cast<float>(computedSize.height) - textHeight) / 2
        );

        // Apply the new position and size to the cursor visual
        d_cursorVisual->position = Position(
            d_textVisual->position->x + static_cast<int32_t>(preCursorTextWidth),
            cursorOffsetY
        );

        d_cursorVisual->customHeight = static_cast<uint32_t>(textHeight);
    }

    void Entry::_calculateSelectionSizeAndOffset() {
        // Proceed only if there is actual selected text
        if (!hasSelectedText()) {
            d_selectionHighlightVisual->customWidth = 1;
            d_selectionHighlightVisual->customHeight = 1;
            d_selectionHighlightVisual->visible = false;
            return;
        }

        // Enable the selection highlight visual
        d_selectionHighlightVisual->visible = true;

        // Get the entry's computed size
        auto computedSize = getComputedSize();

        // Determine the selection start index and size
        uint64_t selectionStartIndex = d_selectionBeganIndex;
        uint64_t selectionEndIndex = d_selectionEndedIndex;

        if (d_selectionBeganIndex > d_selectionEndedIndex) {
            selectionStartIndex = d_selectionEndedIndex;
            selectionEndIndex = d_selectionBeganIndex;
        }

        const auto selectionCharacterSize = selectionEndIndex - selectionStartIndex;

        // Get runtime utility functions
        auto& runtimeFunctions =
            AppManager::getApplicationContext()->getRuntimeUtilityFunctions();

        // Retrieve the function used to calculate
        // minimum required size for the text in pixels.
        auto getTextPixelSizeFn = runtimeFunctions.getTextPixelSizeFn;

        // Calculate the length of the text before the selection
        auto preCursorText = text->substr(0, selectionStartIndex);

        auto [preSelectionTextWidth, textHeight] = getTextPixelSizeFn(
            std::numeric_limits<uint64_t>::max(), computedSize.height,
            preCursorText, font, fontSize, "normal",
            "left", "none"
        );

        // Calculate the selection's y offset from the top of the entry
        auto cursorOffsetY = static_cast<uint32_t>(
            (static_cast<float>(computedSize.height) - textHeight) / 2
        );

        // Apply the new height to the selection visual
        d_selectionHighlightVisual->customHeight =
            static_cast<uint32_t>(textHeight) + cursorOffsetY;

        // Apply the new position to the selection visual
        d_selectionHighlightVisual->position = Position(
            d_textVisual->position->x + static_cast<int32_t>(preSelectionTextWidth),
            cursorOffsetY
        );

        // Calculate the selection pixel width
        auto [selectionPixelWidthF, _0] = getTextPixelSizeFn(
            std::numeric_limits<uint64_t>::max(), computedSize.height,
            text->substr(selectionStartIndex, selectionCharacterSize),
            font, fontSize, "normal", "left", "none"
        );

        if (d_cursorPos > d_selectionBeganIndex) {
            selectionPixelWidthF = static_cast<float>(
                d_cursorVisual->position->x - d_selectionHighlightVisual->position->x
            );
        }

        // Apply the new width to the selection visual
        d_selectionHighlightVisual->customWidth = static_cast<uint32_t>(selectionPixelWidthF);
    }

    uint64_t Entry::_getTextIndexFromMousePosition(int32_t clickPosX) {
        // Calculate local text-relative mouse click position
        int32_t localMousePosX = clickPosX - d_positionInWindow.x;
        localMousePosX +=
                std::abs(d_textVisual->position->x - static_cast<int32_t>(d_textPadding));

        localMousePosX -= d_textPadding * 2;

        // Calculate percentage of the click
        // position in relation to the whole text.
        float percentage =
            static_cast<float>(localMousePosX) / static_cast<float>(d_textVisual->customWidth);

        // Convert clicked coordinate percentage to index in the text
        float clickedIndexF = static_cast<float>(text->size()) * percentage;
        if (clickedIndexF < 0) {
            clickedIndexF = 0;
        }

        // Final index rounding and sanity checks
        uint64_t clickedIndex = static_cast<uint64_t>(std::roundf(clickedIndexF));

        if (clickedIndex > text->size()) {
            clickedIndex = text->size();
        }

        return clickedIndex;
    }
} // namespace mc
