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

    ${cwd}/Color.h
    ${cwd}/Layer.h

    ${cwd}/WidgetContainer.h
    ${cwd}/BaseWidget.h
    ${cwd}/AllWidgets.h

    ${cwd}/Label.h
    ${cwd}/Button.h
    ${cwd}/Checkbox.h
    ${cwd}/Entry.h
    ${cwd}/ProgressBar.h
    ${cwd}/MenuList.h
    ${cwd}/DropdownButton.h
    ${cwd}/Combobox.h
    ${cwd}/StackPanel.h
    ${cwd}/ScrollPanel.h
    ${cwd}/DockPanel.h

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
    ${cwd}/Entry.cpp
    ${cwd}/ProgressBar.cpp
    ${cwd}/MenuList.cpp
    ${cwd}/DropdownButton.cpp
    ${cwd}/Combobox.cpp
    ${cwd}/StackPanel.cpp
    ${cwd}/ScrollPanel.cpp
    ${cwd}/DockPanel.cpp

    PARENT_SCOPE
)
