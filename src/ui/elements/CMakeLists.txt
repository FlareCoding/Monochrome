cmake_minimum_required(VERSION 3.0)

set(
    MC_UIELEMENTS_HEADERS

    src/ui/elements/UILabel.h
    src/ui/elements/UIButton.h
    src/ui/elements/UICheckbox.h
    src/ui/elements/UISlider.h
    src/ui/elements/UITextbox.h
    src/ui/elements/UIScrollPanel.h
    src/ui/elements/UICombobox.h
    src/ui/elements/UITabView.h
    src/ui/elements/UIImage.h
    src/ui/elements/UITextArea.h
    src/ui/elements/UIProgressBar.h
    src/ui/elements/UICircularProgressBar.h
    src/ui/elements/UIDockPanel.h

    PARENT_SCOPE
)

set(
    MC_UIELEMENTS_SOURCES

    src/ui/elements/UILabel.cpp
    src/ui/elements/UIButton.cpp
    src/ui/elements/UICheckbox.cpp
    src/ui/elements/UISlider.cpp
    src/ui/elements/UITextbox.cpp
    src/ui/elements/UIScrollPanel.cpp
    src/ui/elements/UICombobox.cpp
    src/ui/elements/UITabView.cpp
    src/ui/elements/UIImage.cpp
    src/ui/elements/UITextArea.cpp
    src/ui/elements/UIProgressBar.cpp
    src/ui/elements/UICircularProgressBar.cpp
    src/ui/elements/UIDockPanel.cpp

    PARENT_SCOPE
)
