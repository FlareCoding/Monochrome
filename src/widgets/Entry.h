#pragma once
#include "Label.h"
#include "visuals/RectVisual.h"
#include "visuals/BorderVisual.h"
#include "visuals/TextVisual.h"
#include "visuals/ClipRegionVisual.h"
#include <stack>

namespace mc {
    using entryInputValidatorFn_t = std::function<bool(char&)>;

class Entry : public BaseWidget {
public:
    Entry();

    PropertyObserver<uint32_t>  cornerRadius;
    PropertyObserver<uint32_t>  borderThickness;
    PropertyObserver<Color>     backgroundColor;
    PropertyObserver<Color>     borderColor;
    PropertyObserver<Color>     focusedBorderColor;

    // Text to be displayed in the entry
    PropertyObserver<std::string>   text;

    // Dimmed text that should be displayed when the text is empty
    PropertyObserver<std::string>   placeholder;

    // Color of the text
    PropertyObserver<Color>         textColor;

    // Font of the text displayed
    PropertyObserver<std::string>   font;

    // Size of the text font
    PropertyObserver<uint32_t>      fontSize;

    // Possible options: "bold", "italic", "normal"
    PropertyObserver<std::string>   fontStyle;

    // Specifies whether or not the entry is currently editable
    PropertyObserver<bool>          readOnly;

    // Color of the cursor
    PropertyObserver<Color>         cursorColor;

    // Color of the highlight behind the selected text
    PropertyObserver<Color>         selectionHighlightColor;

    // Sets a function used to validate input
    // characters as they are entered into the entry.
    inline void setInputValidator(entryInputValidatorFn_t fn) { d_entryInputValidator = fn; }

    // Erases any existing text in the entry
    void clear();

    // Returns whether or not the entry currently has text
    bool isEmpty();

    // Returns true if any part of the
    // text inside the entry is selected.
    bool hasSelectedText();

    // Returns the currently selected text. If no
    // text is selected, returns an empty string.
    std::string getSelectedText();

    // Removes the selection from the text in an entry
    void clearSelection();

protected:
    Size _measureSize() override;
    void _onSetComputedSize(const Size& size) override;

private:
    void _createVisuals();
    void _setupProperties();

    Shared<RectVisual>       d_bodyVisual;
    Shared<BorderVisual>     d_borderVisual;
    Shared<RectVisual>       d_selectionHighlightVisual;
    Shared<ClipRegionVisual> d_textClippingRegionVisual;
    Shared<TextVisual>       d_textVisual;
    Shared<RectVisual>       d_cursorVisual;

    void _gainedFocus(Shared<Event> e);
    void _lostFocus(Shared<Event> e);
    void _onKeyDown(Shared<Event> e);
    void _onMouseDown(Shared<Event> e);
    void _onMouseUp(Shared<Event> e);
    void _onMouseMoved(Shared<Event> e);

    void _onTextAssigned();
    bool _handleShortcut(char cmdChar);

    void _onCharacterAppended(char inputChar, bool ctrlPressed);
    void _onBackspace(bool ctrlPressed);
    void _onLeftArrowKey(bool ctrlPressed, bool shiftPressed);
    void _onRightArrowKey(bool ctrlPressed, bool shiftPressed);

    void _eraseSelectedText();
    void _handleEntryTextChanged();
    void _updateTextFrameSize();
    void _calculateCursorSizeAndOffset();
    void _calculateSelectionSizeAndOffset();
    uint64_t _getTextIndexFromMousePosition(int32_t clickPosX);

private:
    const uint32_t d_textPadding = 5;

    // Position of the cursor in the text string
    uint64_t d_cursorPos = 0;

    // Validator function used to determine if and which
    // character can be successfully entered into the entry.
    entryInputValidatorFn_t d_entryInputValidator = nullptr;

    // Used in the mouseMoved handler to determine
    // if the mouse was dragging the cursor.
    bool d_mousePressed = false;

    // Used to track position of the entry
    // relative to the window's origin.
    Position d_positionInWindow;

    // Used to track where the entry was clicked
    // in order to determine the size of the selection.
    uint64_t d_selectionBeganIndex = 0;

    // Used to determine where the selection ends.
    uint64_t d_selectionEndedIndex = 0;

    // Used to track changes to the text and
    // used in the 'undo' operation handler.
    std::stack<std::string> d_history;

    // Used to track 'undo' operations and
    // used int he 'redo' operation handler.
    std::stack<std::string> d_undoHistory;
};
} // namespace mc
