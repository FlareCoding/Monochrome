set(CMAKE_CURRENT_SOURCE_DIR src/widgets)
set(cwd ${CMAKE_CURRENT_SOURCE_DIR})

set(
    WIDGETS_HEADERS

    ${cwd}/Color.h
    ${cwd}/Layer.h
    ${cwd}/visuals/Visuals.h
    ${cwd}/visuals/VisualElement.h
    ${cwd}/visuals/RectVisual.h
    ${cwd}/visuals/BorderVisual.h
    ${cwd}/visuals/TextVisual.h
    ${cwd}/WidgetContainer.h
    ${cwd}/BaseWidget.h
    ${cwd}/AllWidgets.h
    ${cwd}/Label.h
    ${cwd}/Button.h

    PARENT_SCOPE
)

set(
    WIDGETS_SOURCES

    ${cwd}/Color.cpp
    ${cwd}/BaseWidget.cpp
    ${cwd}/Label.cpp
    ${cwd}/Button.cpp

    PARENT_SCOPE
)
