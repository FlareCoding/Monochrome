#include "Checkbox.h"

namespace mc {
        Checkbox::Checkbox() {
            appendAllowedEvent("valueChanged");

            _setupProperties();
        }

        void Checkbox::_setupProperties() {
            checked = false;
            checked.forwardEmittedEvents(this);

            displayCheckmark = true;
            displayCheckmark.forwardEmittedEvents(this);

            textMargin = 10;
            textMargin.forwardEmittedEvents(this);

            text = "Button";
            text.forwardEmittedEvents(this);

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

            checkedColor = Color(34, 219, 240);
            checkedColor.forwardEmittedEvents(this);

            checkmarkColor = Color::black;
            checkmarkColor.forwardEmittedEvents(this);

            borderSize = 2;
            borderSize.forwardEmittedEvents(this);

            filled = true;
            filled.forwardEmittedEvents(this);

            stroke = 0;
            stroke.forwardEmittedEvents(this);

            cornerRadius = 4;
            cornerRadius.forwardEmittedEvents(this);

            cursorType = CursorType::Hand;
            size = { 80, 34 };
            cornerRadius = 1;
            color = Color::white;
            alignment = "left";

            on("clicked", [this](auto e) {
                checked = !checked;

                fireEvent("valueChanged", {
                    { "state", checked.get()}
                });
            });
        }
} // namespace mc
