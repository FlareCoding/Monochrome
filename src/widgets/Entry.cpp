#include "Entry.h"

namespace mc {
    Entry::Entry() {
        _createVisuals();
        _setupProperties();
    }

    Size Entry::_measureSize() {
        auto textSize = d_textVisual->measureSize();

        return Size(fixedWidth, textSize.height + 8);
    }

    void Entry::_onSetComputedSize(const Size& size) {
        auto minimumTextVisualWidth = size.width - d_textPadding * 2;

        // Make sure that the area that displays text is
        // always at least the available size inside the entry.
        if (d_textVisual->customWidth < minimumTextVisualWidth) {
            d_textVisual->customWidth = minimumTextVisualWidth;
        }
    }

    void Entry::_createVisuals() {
        // Entry's body rect
        d_bodyVisual = MakeRef<RectVisual>();
        cornerRadius.forwardAssignment(&d_bodyVisual->cornerRadius);
        backgroundColor.forwardAssignment(&d_bodyVisual->color);
        addCoreVisualElement(d_bodyVisual);

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

        // Entry's border
        d_borderVisual = MakeRef<BorderVisual>();
        cornerRadius.forwardAssignment(&d_borderVisual->cornerRadius);
        borderColor.forwardAssignment(&d_borderVisual->color);
        borderThickness.forwardAssignment(&d_borderVisual->thickness);
        addCoreVisualElement(d_borderVisual);
    }

    void Entry::_setupProperties() {
        on("gainedFocus", &Entry::_gainedFocus, this);
        on("lostFocus", &Entry::_lostFocus, this);

        fontSize.on("propertyChanged", [this](Shared<Event> e) {
            // Since the text will be taking up a potentially different
            // amount of space, the layout needs to be recalculated.
            fireEvent("layoutChanged", Event::empty);
        });

        font.on("propertyChanged", [this](Shared<Event> e) {
            // Since the text will be taking up a potentially different
            // amount of space, the layout needs to be recalculated.
            fireEvent("layoutChanged", Event::empty);
        });

        borderColor = Color::black;
        borderColor.forwardEmittedEvents(this);

        focusedBorderColor = Color::blue;
        focusedBorderColor.forwardEmittedEvents(this);

        backgroundColor = Color::white;
        backgroundColor.forwardEmittedEvents(this);

        textColor = Color::black;
        textColor.forwardEmittedEvents(this);

        cornerRadius = 2;
        cornerRadius.forwardEmittedEvents(this);

        borderThickness = 2;
        borderThickness.forwardEmittedEvents(this);

        // The following properties correspond
        // to default values in the text visual.
        font.forwardEmittedEvents(this);
        fontSize.forwardEmittedEvents(this);
        fontStyle.forwardEmittedEvents(this);

        cursorType = CursorType::IBeam;
        cornerRadius = 2;
        fixedWidth = 160;
        text = "Enter your text...";
        fontSize = 14;
    }

    void Entry::_gainedFocus(Shared<Event> e) {
        d_borderVisual->color = focusedBorderColor;
        fireEvent("propertyChanged", Event::empty);
    }

    void Entry::_lostFocus(Shared<Event> e) {
        d_borderVisual->color = backgroundColor;
        fireEvent("propertyChanged", Event::empty);
    }
} // namespace mc
