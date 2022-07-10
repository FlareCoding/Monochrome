set(CMAKE_CURRENT_SOURCE_DIR src/utils)
set(cwd ${CMAKE_CURRENT_SOURCE_DIR})

set(
    UTILS_HEADERS

    ${cwd}/uuid.h
    ${cwd}/Clipboard.h
    ${cwd}/TextUtils.h

    PARENT_SCOPE
)

set(
    UTILS_SOURCES

    ${cwd}/uuid.cpp
    ${cwd}/TextUtils.cpp

    PARENT_SCOPE
)
