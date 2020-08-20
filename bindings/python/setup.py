from distutils.core import setup, Extension

mod_sources = [
        "src\\monochrome_module.cpp",
        "src\\window\\uiwindow_bindings.cpp",
        "src\\ui\\iresponder_bindings.cpp",
        "src\\ui\\uiview_bindings.cpp",
        "src\\ui\\uilabel_bindings.cpp",
        "src\\ui\\uibutton_bindings.cpp",
]

mod_include_dirs = [
        "C:\\Users\\alber\\Desktop\\Monochrome\\src",
]

mod_library_dirs = [
        "C:\\Users\\alber\\Desktop\\Monochrome\\build\\Release"
]

mod_libraries = [
        "user32",
        "ole32",
        "oleaut32",
        "Monochrome"
]

module = Extension('Monochrome', sources=mod_sources, include_dirs=mod_include_dirs, library_dirs=mod_library_dirs, libraries=mod_libraries)
 
setup (name = 'Monochrome',
        version = '8.2',
        description = 'This is a package for monochrome python bindings',
        ext_modules = [module])