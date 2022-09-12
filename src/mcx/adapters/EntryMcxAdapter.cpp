#include "EntryMcxAdapter.h"
#include <widgets/Entry.h>

namespace mc::mcx {
    Shared<BaseWidget> EntryMcxAdapter::createWidgetInstance() {
        return MakeRef<Entry>();
    }

    void EntryMcxAdapter::applyProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        auto entry = std::static_pointer_cast<Entry>(widget);
        entry->textColor =
            mcxNode->getColorAttribute("textColor", entry->textColor);

        entry->text =
            mcxNode->getAttribute("text", entry->text);

        entry->placeholder =
            mcxNode->getAttribute("placeholder", entry->placeholder);

        entry->font =
            mcxNode->getAttribute("font", entry->font);

        entry->fontSize =
            mcxNode->getUIntAttribute("fontSize", entry->fontSize);

        entry->fontStyle =
            mcxNode->getAttribute("fontStyle", entry->fontStyle);

        entry->cornerRadius =
            mcxNode->getUIntAttribute("cornerRadius", entry->cornerRadius);

        entry->borderThickness =
            mcxNode->getUIntAttribute("borderThickness", entry->borderThickness);

        entry->backgroundColor =
            mcxNode->getColorAttribute("backgroundColor", entry->backgroundColor);

        entry->borderColor =
            mcxNode->getColorAttribute("borderColor", entry->borderColor);

        entry->focusedBorderColor =
            mcxNode->getColorAttribute("focusedBorderColor", entry->focusedBorderColor);

        entry->cursorColor =
            mcxNode->getColorAttribute("cursorColor", entry->cursorColor);

        entry->selectionHighlightColor =
            mcxNode->getColorAttribute("highlightColor", entry->selectionHighlightColor);

        entry->readOnly =
            mcxNode->getBoolAttribute("readOnly", entry->readOnly);
    }
} //namespace mc::mcx
