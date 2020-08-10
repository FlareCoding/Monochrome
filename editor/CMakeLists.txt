cmake_minimum_required(VERSION 3.0)

set(
    MC_EDITOR_HEADERS

    panels/UIElementProperties.h
    panels/LabelProperties.h
    panels/ButtonProperties.h
    panels/PlainViewProperties.h
    panels/VariableCodeProperties.h

    utils/EditorUtils.h
    utils/ProjectGenerator.h
    utils/ProjectWindowScriptController.h

    rapidxml/rapidxml.hpp
    rapidxml/rapidxml_iterators.hpp
    rapidxml/rapidxml_print.hpp
    rapidxml/rapidxml_utils.hpp

    Editor.h
)

set(
    MC_EDITOR_SOURCES
   
    panels/LabelProperties.cpp
    panels/ButtonProperties.cpp
    panels/PlainViewProperties.cpp
    panels/VariableCodeProperties.cpp

    utils/EditorUtils.cpp
    utils/ProjectGenerator.cpp
    utils/ProjectWindowScriptController.cpp

    Editor.cpp
    main.cpp
)

add_executable(
    MCEditor 

    ${MC_EDITOR_HEADERS}
    ${MC_EDITOR_SOURCES}
)

target_link_libraries(MCEditor PUBLIC Monochrome)

set (
    SCRIPT_DEPENDENCIES

    template_project_source.h
    template_project_source.cpp
    project_source_generator.py
    mclayout_reader.py
)

foreach(file_i ${SCRIPT_DEPENDENCIES})
    
# Debug
add_custom_command(
        TARGET MCEditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/editor/${file_i} 
                ${CMAKE_CURRENT_BINARY_DIR}/Debug/${file_i}
)

# Release
add_custom_command(
        TARGET MCEditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/editor/${file_i} 
                ${CMAKE_CURRENT_BINARY_DIR}/Release/${file_i}
)

endforeach(file_i)

