set(CMAKE_CURRENT_SOURCE_DIR src/widgets)
set(cwd ${CMAKE_CURRENT_SOURCE_DIR})

set(
    WIDGETS_HEADERS

    ${cwd}/visuals/Visuals.h
    ${cwd}/visuals/VisualElement.h
    ${cwd}/visuals/RectVisual.h
    ${cwd}/visuals/BorderVisual.h
    ${cwd}/visuals/TextVisual.h
    ${cwd}/visuals/ClipRegionVisual.h
    ${cwd}/visuals/ImageVisual.h

    ${cwd}/Color.h
    ${cwd}/Layer.h

    ${cwd}/WidgetContainer.h
    ${cwd}/BaseWidget.h
    ${cwd}/AllWidgets.h

    ${cwd}/Label.h
    ${cwd}/Button.h
    ${cwd}/Checkbox.h
    ${cwd}/Expander.h
    ${cwd}/Image.h
    ${cwd}/MenuList.h
    ${cwd}/StackPanel.h

    PARENT_SCOPE
)

set(
    WIDGETS_SOURCES

    ${cwd}/visuals/TextVisual.cpp

    ${cwd}/Color.cpp
    ${cwd}/BaseWidget.cpp

    ${cwd}/Label.cpp
    ${cwd}/Button.cpp
    ${cwd}/Checkbox.cpp
    ${cwd}/Expander.cpp
    ${cwd}/Image.cpp
    ${cwd}/MenuList.cpp
    ${cwd}/StackPanel.cpp

    PARENT_SCOPE
)
