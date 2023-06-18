#include "EntryMcxAdapter.h"
#include <widgets/Entry.h>

namespace mc::mcx {
    Shared<BaseWidget> EntryMcxAdapter::createWidgetInstance(Shared<McxNode>& mcxNode) {
        return MakeRef<Entry>();
    }

    void EntryMcxAdapter::extractProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        auto entry = std::static_pointer_cast<Entry>(widget);

        mcxNode->setAttribute("textColor", _toString(entry->textColor));
        mcxNode->setAttribute("text", entry->text);
        mcxNode->setAttribute("placeholder", entry->placeholder);
        mcxNode->setAttribute("font", entry->font);
        mcxNode->setAttribute("fontStyle", entry->fontStyle);
        mcxNode->setAttribute("fontSize", _toString(entry->fontSize));
        mcxNode->setAttribute("cornerRadius", _toString(entry->cornerRadius));
        mcxNode->setAttribute("borderThickness", _toString(entry->borderThickness));
        mcxNode->setAttribute("backgroundColor", _toString(entry->backgroundColor));
        mcxNode->setAttribute("borderColor", _toString(entry->borderColor));
        mcxNode->setAttribute("focusedBorderColor", _toString(entry->focusedBorderColor));
        mcxNode->setAttribute("cursorColor", _toString(entry->cursorColor));
        mcxNode->setAttribute("highlightColor", _toString(entry->selectionHighlightColor));
        mcxNode->setAttribute("readOnly", _toString(entry->readOnly));
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

    std::vector<std::string> EntryMcxAdapter::getAvailableProperties() {
        static const std::vector<std::string> props = {
            "textColor", "text", "placeholder",
            "readOnly", "font", "fontSize", "fontStyle",
            "cornerRadius", "borderThickness",
            "backgroundColor", "borderColor",
            "focusedBorderColor", "cursorColor",
            "highlightColor"
        };

        return props;
    }
} //namespace mc::mcx
