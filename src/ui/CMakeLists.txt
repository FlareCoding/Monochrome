cmake_minimum_required(VERSION 3.0)

add_subdirectory(elements)
add_subdirectory(animations)

set(
    MC_UI_HEADERS

    ${MC_UIELEMENTS_HEADERS}
    ${MC_ANIMATIONS_HEADERS}

    src/ui/IDrawable.h
    src/ui/Layer.h
    src/ui/Anchor.h
    src/ui/UIElements.h
    src/ui/UIView.h
    src/ui/UIFileDialogue.h
    src/ui/UICursor.h
    src/ui/UITimer.h

    PARENT_SCOPE
)

set(
    MC_UI_SOURCES

    ${MC_UIELEMENTS_SOURCES}
    ${MC_ANIMATIONS_SOURCES}

    src/ui/UIView.cpp
    src/ui/UIFileDialogue.cpp
    src/ui/UICursor.cpp
    src/ui/UITimer.cpp

    PARENT_SCOPE
)
