set(CMAKE_CURRENT_SOURCE_DIR src/application)
set(cwd ${CMAKE_CURRENT_SOURCE_DIR})

set(
    APPLICATION_HEADERS

    ${cwd}/AppManager.h
    ${cwd}/ApplicationContext.h
    
    PARENT_SCOPE
)

set(
    APPLICATION_SOURCES

    ${cwd}/AppManager.cpp
    ${cwd}/ApplicationContext.cpp

    PARENT_SCOPE
)
