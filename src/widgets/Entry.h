#pragma once
#include "BaseWidget.h"
#include <events/KeyboardEvents.h>
#include <stack>

namespace mc
{
    using entryInputValidatorFn_t = std::function<bool(char&)>;

    class Entry : public BaseWidget
    {
    public:
        Entry();
        inline const std::string getType() const override { return "entry"; }

        // Text to be displayed on the entry
        PropertyObserver<std::string>   text;

        // Placeholder to be displayed when the entry is empty
        PropertyObserver<std::string>   placeholder;

        // Font of the text displayed
        PropertyObserver<std::string>   font;

        // Size of the text font
        PropertyObserver<uint32_t>      fontSize;

        // Possible options: "bold", "italic", "normal"
        PropertyObserver<std::string>   fontStyle;

        // Color of the border around the entry, transparent by default
        PropertyObserver<Color>         borderColor;

        // Width of the border around the entry's body
        PropertyObserver<uint32_t>      borderSize;

        // Color of the border around the entry when it is focused
        PropertyObserver<Color>         focusedHighlightColor;

        // Color of the selected text background
        PropertyObserver<Color>         selectionColor;

        // Color of the text cursor when entry is activated
        PropertyObserver<Color>         cursorColor;

        // Specifies whether or not to fill the entry's body
        PropertyObserver<bool>          filled;

        // Specifies the width of the frame if 'filled' is false
        PropertyObserver<uint32_t>      stroke;

        // Corner radius of the entry's frame
        PropertyObserver<uint32_t>      cornerRadius;

        // Erases any existing text in the entry
        void clear();

        // Returns whether or not the entry currently has text.
        bool isEmpty();

        // Returns true if any part of the
        // text inside the entry is selected.
        bool hasSelectedText() const;

        // Returns the currently selected text. If no
        // text is selected, returns an empty string.
        std::string getSelectedText();

        // Removes the selection from the text in an entry
        void clearSelection();

        // Sets a function used to validate input
        // characters as they are entered into the entry.
        inline void setInputValidator(entryInputValidatorFn_t fn) { d_entryInputValidator = fn; }

    private:
        void _setupProperties() override;
        void _setupEventHandlers();

    private:
        void _onKeyPressed(Shared<KeyDownEvent> e);
        void _onMousePressed(int32_t clickPosX);
        void _onMouseMoved(int32_t mousePosX);

        void _onTextAssigned();
        void _onCharacterAppened(char inputChar, bool ctrlPressed);
        void _onBackspace(bool ctrlPressed);
        void _onRightArrowKey(bool ctrlPressed, bool shiftPressed);
        void _onLeftArrowKey(bool ctrlPressed, bool shiftPressed);
        bool _handleShortcut(char cmdChar);
        void _eraseSelectedText();
        void _handleEntryTextChanged();

        void _updateTextFrameSize();

        uint64_t _getTextIndexFromMousePosition(int32_t mousePos);

    private:
        const uint32_t d_entryTextPadding = 6;

        // Position of the cursor in the string
        uint64_t d_cursorPos = 0;

        // Movable frame around the text that's invisible
        // and used only for text position calculations.
        Frame d_textFrame;

        // Used in the mouseMoved handler to determine
        // if the mouse was dragging the cursor.
        bool d_mousePressed = false;

        // Used to track where the entry was clicked
        // in order to determine the size of the selection.
        uint64_t d_selectionBeganIndex = 0;

        // Used to determine where the selection ends
        uint64_t d_selectionEndedIndex = 0;

        // Validator function used to determine if and which
        // character can be successfully entered into the entry.
        entryInputValidatorFn_t d_entryInputValidator = nullptr;

        // Used to track changes to the text and
        // used in the 'undo' operation handler.
        std::stack<std::string> d_history;

        // Used to track 'undo' operations and
        // used in the 'redo' operation handler.
        std::stack<std::string> d_undoHistory;

    private:
        friend class Renderer;
    };
}
