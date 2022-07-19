set(CMAKE_CURRENT_SOURCE_DIR src/widgets)
set(cwd ${CMAKE_CURRENT_SOURCE_DIR})

set(
    WIDGETS_HEADERS

    ${cwd}/Layer.h
    ${cwd}/Color.h
    ${cwd}/WidgetHostController.h
    ${cwd}/AllWidgets.h
    ${cwd}/BaseWidget.h
    ${cwd}/BaseContainerWidget.h
    ${cwd}/Panel.h
    ${cwd}/Label.h
    ${cwd}/Button.h
    ${cwd}/Entry.h
    ${cwd}/MenuList.h
    ${cwd}/DropdownButton.h
    ${cwd}/Combobox.h
    ${cwd}/FlowPanel.h
    ${cwd}/CustomRenderedWidget.h
    ${cwd}/WidgetControlFrame.h

    PARENT_SCOPE
)

set(
    WIDGETS_SOURCES

    ${cwd}/Color.cpp
    ${cwd}/WidgetHostController.cpp
    ${cwd}/BaseWidget.cpp
    ${cwd}/BaseContainerWidget.cpp
    ${cwd}/Panel.cpp
    ${cwd}/Label.cpp
    ${cwd}/Button.cpp
    ${cwd}/Entry.cpp
    ${cwd}/MenuList.cpp
    ${cwd}/DropdownButton.cpp
    ${cwd}/Combobox.cpp
    ${cwd}/FlowPanel.cpp
    ${cwd}/WidgetControlFrame.cpp

    PARENT_SCOPE
)
