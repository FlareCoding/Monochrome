#include "Label.h"

namespace mc
{
    Label::Label() {        
        _setupProperties();
    }

    void Label::_setupProperties() {
        text = "Label";
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

        size = { 80, 20 };
    }
}
