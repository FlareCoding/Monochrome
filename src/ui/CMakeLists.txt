cmake_minimum_required(VERSION 3.0)

add_subdirectory(elements)

set(
    MC_UI_HEADERS

    ${MC_UIELEMENTS_HEADERS}

    src/ui/IDrawable.h
    src/ui/Layer.h
    src/ui/UIElements.h
    src/ui/UIView.h

    PARENT_SCOPE
)

set(
    MC_UI_SOURCES

    ${MC_UIELEMENTS_SOURCES}
    src/ui/UIView.cpp

    PARENT_SCOPE
)
