#include "Button.h"

namespace mc {
    Button::Button() {
        _setupProperties();
    }

    void Button::_setupProperties() {
        text = "Button";
        text.forwardEmittedEvents(this);

        secondaryRightText = "";
        secondaryRightText.forwardEmittedEvents(this);

        secondaryLeftText = "";
        secondaryLeftText.forwardEmittedEvents(this);

        font = "arial";
        font.forwardEmittedEvents(this);

        fontSize = 14;
        fontSize.forwardEmittedEvents(this);

        fontStyle = "normal";
        fontStyle.forwardEmittedEvents(this);

        alignment = "center";
        alignment.forwardEmittedEvents(this);

        wordWrapMode = "word";
        wordWrapMode.forwardEmittedEvents(this);

        borderColor = Color::transparent;
        borderColor.forwardEmittedEvents(this);

        borderSize = 2;
        borderSize.forwardEmittedEvents(this);

        filled = true;
        filled.forwardEmittedEvents(this);

        stroke = 0;
        stroke.forwardEmittedEvents(this);

        cornerRadius = 4;
        cornerRadius.forwardEmittedEvents(this);

        onHoverColor = Color::transparent;
        onHoverColor.forwardEmittedEvents(this);

        onMousePressColor = Color::transparent;
        onMousePressColor.forwardEmittedEvents(this);

        cursorType = CursorType::Hand;
        size = { 100, 24 };
    }
} // namespace mc
