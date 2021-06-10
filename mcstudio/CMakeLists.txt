cmake_minimum_required(VERSION 3.0)

set(
    MC_STUDIO_HEADERS

    Studio.h
    ProjectView.h
    StudioUtils.h
    StudioSettings.h
    WidgetProperties.h
)

set(
    MC_STUDIO_SOURCES

    Studio.cpp
    ProjectView.cpp
    StudioUtils.cpp
    StudioSettings.cpp
    WidgetProperties.cpp
    main.cpp
)

add_executable(
    MonochromeStudio 

    ${MC_STUDIO_HEADERS}
    ${MC_STUDIO_SOURCES}
)

target_link_libraries(MonochromeStudio PUBLIC Monochrome)
