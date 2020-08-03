cmake_minimum_required(VERSION 3.0)

set(
    MC_UIELEMENTS_HEADERS

    src/ui/elements/UILabel.h
    src/ui/elements/UIButton.h
    src/ui/elements/UICheckbox.h
    src/ui/elements/UISlider.h
    src/ui/elements/UITextbox.h
    src/ui/elements/UIScrollPanel.h
    src/ui/elements/UICombobox.h
    src/ui/elements/UITabView.h

    PARENT_SCOPE
)

set(
    MC_UIELEMENTS_SOURCES

    src/ui/elements/UILabel.cpp
    src/ui/elements/UIButton.cpp
    src/ui/elements/UICheckbox.cpp
    src/ui/elements/UISlider.cpp
    src/ui/elements/UITextbox.cpp
    src/ui/elements/UIScrollPanel.cpp
    src/ui/elements/UICombobox.cpp
    src/ui/elements/UITabView.cpp

    PARENT_SCOPE
)
