cmake_minimum_required(VERSION 3.0)

set(
    MC_UIELEMENTS_HEADERS

    src/ui/elements/UILabel.h
    src/ui/elements/UIButton.h
    src/ui/elements/UICheckbox.h
    src/ui/elements/UISlider.h

    PARENT_SCOPE
)

set(
    MC_UIELEMENTS_SOURCES

    src/ui/elements/UILabel.cpp
    src/ui/elements/UIButton.cpp
    src/ui/elements/UICheckbox.cpp
    src/ui/elements/UISlider.cpp

    PARENT_SCOPE
)
