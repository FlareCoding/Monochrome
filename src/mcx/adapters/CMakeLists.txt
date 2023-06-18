set(CMAKE_CURRENT_SOURCE_DIR src/mcx/adapters)
set(cwd ${CMAKE_CURRENT_SOURCE_DIR})

set(
    MCX_ADAPTERS_HEADERS

    ${cwd}/McxParsingAdapter.h
    ${cwd}/BaseWidgetMcxAdapter.h
    ${cwd}/StackPanelMcxAdapter.h
    ${cwd}/DockPanelMcxAdapter.h
    ${cwd}/ScrollPanelMcxAdapter.h
    ${cwd}/SplitterPanelMcxAdapter.h
    ${cwd}/LabelMcxAdapter.h
    ${cwd}/ButtonMcxAdapter.h
    ${cwd}/CheckboxMcxAdapter.h
    ${cwd}/ComboboxMcxAdapter.h
    ${cwd}/DropdownButtonMcxAdapter.h
    ${cwd}/MenuListMcxAdapter.h
    ${cwd}/EntryMcxAdapter.h
    ${cwd}/ProgressBarMcxAdapter.h
    ${cwd}/TreeViewGroupMcxAdapter.h
    ${cwd}/TreeViewMcxAdapter.h
    ${cwd}/TabViewMcxAdapter.h
    ${cwd}/ImageMcxAdapter.h
    ${cwd}/UserWidgetMcxAdapter.h

    PARENT_SCOPE
)

set(
    MCX_ADAPTERS_SOURCES

    ${cwd}/BaseWidgetMcxAdapter.cpp
    ${cwd}/StackPanelMcxAdapter.cpp
    ${cwd}/DockPanelMcxAdapter.cpp
    ${cwd}/ScrollPanelMcxAdapter.cpp
    ${cwd}/SplitterPanelMcxAdapter.cpp
    ${cwd}/LabelMcxAdapter.cpp
    ${cwd}/ButtonMcxAdapter.cpp
    ${cwd}/CheckboxMcxAdapter.cpp
    ${cwd}/ComboboxMcxAdapter.cpp
    ${cwd}/DropdownButtonMcxAdapter.cpp
    ${cwd}/MenuListMcxAdapter.cpp
    ${cwd}/EntryMcxAdapter.cpp
    ${cwd}/ProgressBarMcxAdapter.cpp
    ${cwd}/TreeViewGroupMcxAdapter.cpp
    ${cwd}/TreeViewMcxAdapter.cpp
    ${cwd}/TabViewMcxAdapter.cpp
    ${cwd}/ImageMcxAdapter.cpp
    ${cwd}/UserWidgetMcxAdapter.cpp

    PARENT_SCOPE
)
