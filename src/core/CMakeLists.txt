set(CMAKE_CURRENT_SOURCE_DIR src/core)
set(cwd ${CMAKE_CURRENT_SOURCE_DIR})

set(
    CORE_HEADERS

    ${cwd}/Core.h
    ${cwd}/InternalFlags.h
    ${cwd}/StackTracer.h
    ${cwd}/FunctionProfiler.h

    PARENT_SCOPE
)

set(
    CORE_SOURCES

    ${cwd}/Core.cpp
    ${cwd}/InternalFlags.cpp
    ${cwd}/StackTracer.cpp
    ${cwd}/FunctionProfiler.cpp

    PARENT_SCOPE
)
