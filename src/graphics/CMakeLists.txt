cmake_minimum_required(VERSION 3.0)

set(
    MC_GRAPHICS_HEADERS
    
    src/graphics/d2d_resources/CoreResources.h
    src/graphics/Color.h
    src/graphics/Graphics.h
    src/graphics/RenderTarget.h
    src/graphics/Shapes.h
    src/graphics/TextProperties.h
    src/graphics/Bitmap.h

    PARENT_SCOPE
)

set(
    MC_GRAPHICS_SOURCES

    src/graphics/d2d_resources/CoreResources.cpp
    src/graphics/Color.cpp
    src/graphics/Graphics.cpp
    src/graphics/RenderTarget.cpp
    src/graphics/Bitmap.cpp

    PARENT_SCOPE
)
