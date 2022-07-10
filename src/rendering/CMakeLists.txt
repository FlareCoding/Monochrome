set(CMAKE_CURRENT_SOURCE_DIR src/rendering)
set(cwd ${CMAKE_CURRENT_SOURCE_DIR})

set(
    RENDERING_HEADERS

    ${cwd}/RenderTarget.h
    ${cwd}/Renderer.h

    PARENT_SCOPE
)

set(
    RENDERING_SOURCES
    
    ${cwd}/Renderer.cpp

    PARENT_SCOPE
)
