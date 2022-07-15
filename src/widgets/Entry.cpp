#include "Entry.h"
#include <core/InternalFlags.h>
#include <application/AppManager.h>
#include <events/MouseEvents.h>
#include <utils/Clipboard.h>
#include <cmath>

namespace mc
{
    Entry::Entry() {
        // Entry-specific events
        appendAllowedEvent("change");
        appendAllowedEvent("enter");

        _setupProperties();
        _setupEventHandlers();
    }

    void Entry::clear() {
        text = "";
    }

    bool Entry::isEmpty() {
        return !text->size();
    }

    bool Entry::hasSelectedText() const {
        return d_selectionEndedIndex != d_selectionBeganIndex;
    }

    std::string Entry::getSelectedText() {
        uint64_t selectionLength = std::llabs(
            (int64_t)d_selectionEndedIndex - (int64_t)d_selectionBeganIndex);

        if (d_selectionEndedIndex > d_selectionBeganIndex) {
            return text->substr(d_selectionBeganIndex, selectionLength);
        }
        else {
            return text->substr(d_selectionEndedIndex, selectionLength);
        }
    }

    void Entry::clearSelection() {
        d_selectionBeganIndex = d_cursorPos;
        d_selectionEndedIndex = d_cursorPos;
    }

    void Entry::_setupProperties() {
        text = "";
        text.on("propertyChanged", [this](auto e) {
            _onTextAssigned();
        });
        text.forwardEmittedEvents(this);

        placeholder = "Type something...";
        placeholder.forwardEmittedEvents(this);

        readOnly = false;
        readOnly.forwardEmittedEvents(this);

        font = "arial";
        font.forwardEmittedEvents(this);

        fontSize = 14;
        fontSize.forwardEmittedEvents(this);

        fontStyle = "normal";
        fontStyle.forwardEmittedEvents(this);

        borderColor = Color::black;
        borderColor.forwardEmittedEvents(this);

        borderSize = 4;
        borderSize.forwardEmittedEvents(this);

        focusedHighlightColor = Color(60, 60, 255);
        focusedHighlightColor.forwardEmittedEvents(this);

        selectionColor = Color(0, 255, 0, 80);
        selectionColor.forwardEmittedEvents(this);
        
        cursorColor = Color::black;
        cursorColor.forwardEmittedEvents(this);

        filled = true;
        filled.forwardEmittedEvents(this);

        stroke = 0;
        stroke.forwardEmittedEvents(this);

        cornerRadius = 0;
        cornerRadius.forwardEmittedEvents(this);

        cursorType = CursorType::IBeam;
        size = { 140, 24 };
        backgroundColor = Color::white;
        d_history.push(text);

        d_textFrame.position.x = d_entryTextPadding;
    }

    void Entry::_setupEventHandlers() {
        on("keyDown", [this](Shared<Event> e) {
            _onKeyPressed(std::static_pointer_cast<KeyDownEvent>(e));
        });

        on("mouseDown", [this](Shared<Event> e) {
            d_mousePressed = true;

            auto clickedEvent = std::static_pointer_cast<MouseButtonEvent>(e);

            auto clickPosX = clickedEvent->getLocation().x;
            _onMousePressed(clickPosX);
        });
        
        on("mouseUp", [this](Shared<Event> e) {
            d_mousePressed = false;
        });

        on("mouseMoved", [this](Shared<Event> e) {
            // Only react to mouse "dragging"
            if (!d_mousePressed) {
                return;
            }

            auto mouseMovedEvent = std::static_pointer_cast<MouseMovedEvent>(e);
            int32_t mousePosX = mouseMovedEvent->getLocation().x;

            _onMouseMoved(mousePosX);
        });
    }
    
    void Entry::_onKeyPressed(Shared<KeyDownEvent> e) {
        KeyCode keycode = e->getKeyCode();
        bool ctrlPressed = e->isCtrlPressed();
        bool shiftPressed = e->isShiftPressed();

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
            fireEvent("enter", MakeRef<Event>(eventDataMap_t{
                { "text", text.get() }
            }));
            break;
        }
        default: {
            char inputChar = e->getChar();
            if (inputChar) {
                _onCharacterAppened(inputChar, ctrlPressed);
            }
            break;
        }
        }
    }

    void Entry::_onMousePressed(int32_t clickPosX) {
        if (text->empty()) {
            return;
        }

        d_cursorPos = _getTextIndexFromMousePosition(clickPosX);

        _updateTextFrameSize();
        clearSelection();
    }

    void Entry::_onMouseMoved(int32_t mousePosX) {
        d_cursorPos = _getTextIndexFromMousePosition(mousePosX);

        d_selectionEndedIndex = d_cursorPos;

        _updateTextFrameSize();
        fireEvent("propertyChanged", Event::empty);
    }
    
    void Entry::_onTextAssigned() {
        // Validating assigned text
        if (d_entryInputValidator) {
            for (uint64_t i = 0; i < text->size(); ++i) {
                if (!d_entryInputValidator(text->at(i))) {
                    return;
                }
            }
        }

        d_cursorPos = text->size();
        clearSelection();
        
        _updateTextFrameSize();

        // Clear the undo/redo history
        d_history = std::stack<std::string>();
        d_undoHistory = std::stack<std::string>();
    }

    void Entry::_onCharacterAppened(char inputChar, bool ctrlPressed) {
        // Check for any potential shortcuts
        // such as Ctrl+C, Ctrl+V, etc.
        if (ctrlPressed) {
            bool shortcutHandled = _handleShortcut(inputChar);

            if (shortcutHandled) {
                _updateTextFrameSize();
                fireEvent("propertyChanged", Event::empty);
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

        text->insert(text->begin() + d_cursorPos, inputChar);
        ++d_cursorPos;

        _handleEntryTextChanged();
        _updateTextFrameSize();
        fireEvent("propertyChanged", Event::empty);
    }

    void Entry::_onBackspace(bool ctrlPressed) {
        // Check if the entry has any text selected,
        // because if so, it doesn't matter if CTRL is
        // pressed or not, ony the selection should be deleted.
        if (hasSelectedText()) {
            // Deleting selected text string
            _eraseSelectedText();

            // Reset the selection
            clearSelection();
        }
        else {
            // Do nothing if the cursor is
            // already at the very beginning.
            if (d_cursorPos == 0) {
                return;
            }

            if (ctrlPressed) {
                auto preCursorText = text->substr(0, d_cursorPos);
                auto postCursorText = text->substr(d_cursorPos);

                const auto pos = preCursorText.find_last_of(" \t\n");
                if (pos == std::string::npos) {
                    text->assign(postCursorText);
                    d_cursorPos = 0;
                }
                else {
                    text->assign(text->substr(0, pos) + postCursorText);
                    d_cursorPos = pos;
                }
            }
            else {
                text->erase(d_cursorPos - 1, 1);
                --d_cursorPos;
            }
        }

        _handleEntryTextChanged();
        _updateTextFrameSize();
        fireEvent("propertyChanged", Event::empty);
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
            }
            else {
                d_cursorPos = d_cursorPos + pos;
            }
        }
        else {
            ++d_cursorPos;
        }

        // Check if shift is pressed, and if,
        // so update the selection end index.
        if (shiftPressed) {
            d_selectionEndedIndex = d_cursorPos;
        }
        else {
            clearSelection();
        }

        _updateTextFrameSize();
        fireEvent("propertyChanged", Event::empty);
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
            }
            else {
                d_cursorPos = pos;
            }
        }
        else {
            --d_cursorPos;
        }

        // Check if shift is pressed, and if,
        // so update the selection end index.
        if (shiftPressed) {
            d_selectionEndedIndex = d_cursorPos;
        }
        else {
            clearSelection();
        }

        _updateTextFrameSize();

        fireEvent("propertyChanged", Event::empty);
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

    void Entry::_eraseSelectedText() {
        uint64_t selectionLength = std::llabs(
            (int64_t)d_selectionEndedIndex - (int64_t)d_selectionBeganIndex);

        uint64_t selectionStartIndex = d_selectionBeganIndex;

        // Find the index that indicates the start of the selection
        if (selectionStartIndex > d_selectionEndedIndex) {
            selectionStartIndex = d_selectionEndedIndex;
        }

        // Erase the selected text
        text->erase(selectionStartIndex, selectionLength);
        d_cursorPos = selectionStartIndex;

        clearSelection();
    }

    void Entry::_handleEntryTextChanged() {
        // Update the text history
        d_history.push(text.get());

        fireEvent("change", MakeRef<Event>(eventDataMap_t{
            { "text", text.get() }
        }));
    }

    void Entry::_updateTextFrameSize() {
        // Get runtime utility functions
        auto& runtimeFunctions =
            AppManager::getApplicationContext()->getRuntimeUtilityFunctions();

        // Retrieve the function used to calculate
        // minimum required size for the text in pixels.
        auto getTextPixelSizeFn = runtimeFunctions.getTextPixelSizeFn;

        // Determine the minimum required width for the text in pixels
        auto [textPixelWidth, textPixelHeight] = getTextPixelSizeFn(
            std::numeric_limits<uint64_t>::max(), size->height,
            text, font, fontSize, "normal",
            "left", "none"
        );

        d_textFrame.size = Size(
            static_cast<uint64_t>(textPixelWidth),
            size->height
        );

        // Check if text frame of reference should be moved left,
        // ultimately scrolling the text forward
        if (d_cursorPos == text->size()) {

            int32_t diff = (d_textFrame.position.x + d_textFrame.size.width) - (size->width - d_entryTextPadding);
            
            if (diff > 0) {
                d_textFrame.position.x -= diff;
            }
        }

        // Additionally check if the text fully fits into the entry,
        // then it has to be aligned with the left-most clip region boundary.
        if (textPixelWidth <= (size->width - (d_entryTextPadding * 2))) {
            d_textFrame.position.x = d_entryTextPadding;
        }

        // In case of text deletion from the back, the text frame
        // can remain shifted over to the left, so it needs to be
        // aligned with either the left or the right clip boundaries.
        int32_t rightDiff = (int64_t)(d_textFrame.position.x + d_textFrame.size.width) -
                            (int64_t)(size->width - d_entryTextPadding);

        int32_t leftDiff = (int64_t)(d_entryTextPadding) - (int64_t)d_textFrame.position.x;

        if (leftDiff > 0 && rightDiff < 0) {
            int32_t shiftDiff = std::min(std::abs(rightDiff), std::abs(leftDiff));
            
            d_textFrame.position.x += shiftDiff;
        }

        // Determine if the text needs to be scrolled
        // forward when the cursor is too far back.
        auto [preCursorTextWidth, _0] = getTextPixelSizeFn(
            std::numeric_limits<uint64_t>::max(), size->height,
            text->substr(0, d_cursorPos), font, fontSize, fontStyle,
            "left", "none"
        );

        int32_t preCursorDiff = (d_entryTextPadding - (d_textFrame.position.x + (int32_t)preCursorTextWidth));
        if (preCursorDiff > 0) {
            d_textFrame.position.x += preCursorDiff;
        }

        // If the text is added to the middle of the string,
        // the text needs to be scrolled back only after the
        // cursor reaches the right boundary of the clip region.
        int32_t cursorPoint = d_textFrame.position.x + (int32_t)preCursorTextWidth;
        int32_t rightBoundary = size->width - d_entryTextPadding;
        int32_t postCursorDiff = cursorPoint - rightBoundary;

        if (postCursorDiff > 0) {
            d_textFrame.position.x -= postCursorDiff;
        }
    }
    
    uint64_t Entry::_getTextIndexFromMousePosition(int32_t mousePos) {
        int32_t localMousePosX = mousePos - position->x;
        localMousePosX += std::abs(d_textFrame.position.x - (int32_t)d_entryTextPadding);
        localMousePosX -= d_entryTextPadding;

        float percentage = (float)(localMousePosX) / (float)d_textFrame.size.width;
        float clickedIndex = (float)text->size() * percentage;
        if (clickedIndex < 0) {
            clickedIndex = 0;
        }

        uint64_t result = (uint64_t)std::roundf(clickedIndex);

        // Cursor position sanitation
        if (result > text->size()) {
            result = text->size();
        }

        return result;
    }
}
