set(CMAKE_CURRENT_SOURCE_DIR src/widgets)
set(cwd ${CMAKE_CURRENT_SOURCE_DIR})

set(
    WIDGETS_HEADERS

    ${cwd}/Color.h
    ${cwd}/Layer.h
    ${cwd}/visuals/VisualElement.h
    ${cwd}/visuals/RectVisual.h
    ${cwd}/visuals/TextVisual.h
    ${cwd}/WidgetContainer.h
    ${cwd}/BaseWidget.h

    PARENT_SCOPE
)

set(
    WIDGETS_SOURCES

    ${cwd}/Color.cpp

    PARENT_SCOPE
)
