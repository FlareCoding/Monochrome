set(CMAKE_CURRENT_SOURCE_DIR src/mcx)
set(cwd ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(adapters)

set(
    MCX_HEADERS

    ${cwd}/rapidxml/rapidxml_iterators.hpp
    ${cwd}/rapidxml/rapidxml_print.hpp
    ${cwd}/rapidxml/rapidxml_utils.hpp
    ${cwd}/rapidxml/rapidxml.hpp

    ${cwd}/McxEngine.h
    ${cwd}/McxNode.h

    ${MCX_ADAPTERS_HEADERS}

    PARENT_SCOPE
)

set(
    MCX_SOURCES

    ${cwd}/McxEngine.cpp
    ${cwd}/McxNode.cpp

    ${MCX_ADAPTERS_SOURCES}

    PARENT_SCOPE
)
