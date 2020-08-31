cmake_minimum_required(VERSION 3.0)

set(
    MC_ANIMATIONS_HEADERS

    src/ui/animations/Animations.h
    src/ui/animations/Animation.h
    src/ui/animations/FadeOutAnimation.h
    src/ui/animations/FadeInAnimation.h
    src/ui/animations/TranslationAnimation.h
    src/ui/animations/ScaleAnimation.h

    PARENT_SCOPE
)

set(
    MC_ANIMATIONS_SOURCES

    src/ui/animations/Animation.cpp
    src/ui/animations/FadeOutAnimation.cpp
    src/ui/animations/FadeInAnimation.cpp
    src/ui/animations/TranslationAnimation.cpp
    src/ui/animations/ScaleAnimation.cpp

    PARENT_SCOPE
)
