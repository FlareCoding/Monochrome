from distutils.core import setup, Extension
from distutils import sysconfig
from distutils.core import setup
import platform
import os

if platform.system() == 'Darwin':
    os.environ['LDFLAGS'] = '-framework Cocoa'

os.environ['CFLAGS'] = '-std=c++17'
os.environ['CC'] = 'g++'

if platform.system() == 'Darwin':
    cflags = sysconfig.get_config_var('CFLAGS')
    opt = sysconfig.get_config_var('OPT')
    sysconfig._config_vars['CFLAGS'] = cflags.replace(' -Wall ', ' ')
    sysconfig._config_vars['OPT'] = opt.replace(' -Wall ', ' ')

mod_sources = [
        "src/monochrome_module.cpp",
        "src/core/python_bindings_core.cpp",

        "src/window_bindings/bindings_uiwindow.cpp",
        "src/ui_bindings/bindings_iresponder.cpp",
        "src/ui_bindings/bindings_frame.cpp",
        "src/ui_bindings/bindings_point.cpp",
        "src/ui_bindings/bindings_color.cpp",
        "src/ui_bindings/bindings_layer.cpp",
        "src/ui_bindings/bindings_textproperties.cpp",

        "src/ui_bindings/bindings_uiview.cpp",
        "src/ui_bindings/bindings_uilabel.cpp",
        "src/ui_bindings/bindings_uibutton.cpp",
        "src/ui_bindings/bindings_uicheckbox.cpp",
        "src/ui_bindings/bindings_uislider.cpp",
        "src/ui_bindings/bindings_uitextbox.cpp",
        "src/ui_bindings/bindings_uiscrollpanel.cpp",
        "src/ui_bindings/bindings_uicombobox.cpp",
        "src/ui_bindings/bindings_uitextarea.cpp",
        "src/ui_bindings/bindings_uiprogressbar.cpp",
        "src/ui_bindings/bindings_uicircularprogressbar.cpp",
        "src/ui_bindings/bindings_uitabview.cpp",
        "src/ui_bindings/bindings_uiimage.cpp",

        "src/events_bindings/bindings_mousebuttonpressedevent.cpp",
        "src/events_bindings/bindings_mousebuttonreleasedevent.cpp",
        "src/events_bindings/bindings_mousemovedevent.cpp",
        "src/events_bindings/bindings_mousehoveronevent.cpp",
        "src/events_bindings/bindings_mousehoveroffevent.cpp",
        "src/events_bindings/bindings_keypressedevent.cpp",
        "src/events_bindings/bindings_focuschangedevent.cpp",
        "src/events_bindings/bindings_keyreleasedevent.cpp",
        "src/events_bindings/bindings_windowresizedevent.cpp",
        "src/events_bindings/bindings_windowclosedevent.cpp",
        "src/events_bindings/bindings_windowgainedfocusevent.cpp",
        "src/events_bindings/bindings_windowlostfocusevent.cpp",
        "src/events_bindings/bindings_windowupdatedevent.cpp",
]

mod_include_dirs = [
        "../../src",
]

mod_library_dirs = [
        "../../build"
]

mod_libraries = [
        "Monochrome"
]

module = Extension('Monochrome', 
                sources=mod_sources,
                include_dirs=mod_include_dirs,
                library_dirs=mod_library_dirs,
                libraries=mod_libraries,
                language="c++17")
 
setup (name = 'Monochrome',
        version = '8.2',
        description = 'This is a package for monochrome python bindings',
        ext_modules = [module])