import os
import sys
from mclayout_reader import *

input_TargetPath    = sys.argv[1]
input_ProjectName   = sys.argv[2]
input_ClassName     = sys.argv[3]

reader = MCLayoutReader(input_TargetPath + "\\" + input_ProjectName + ".mc")
reader.generate_cpp_source()

HeaderVariables = {}
HeaderVariables['ClassName'] = input_ClassName

SourceVariables = {}
SourceVariables['ClassName'] = input_ClassName
SourceVariables['UserInterfaceSourceCode'] = reader.get_cpp_source_string()
SourceVariables['WindowCreationSourceCode'] = reader.get_window_creation_source_string()

source_dir = input_TargetPath + "\\src"
if not os.path.exists(source_dir):
    os.makedirs(source_dir)

# Header
with open("template_project_source.h", 'r') as source_header:
    header_template = source_header.read()

with open(input_TargetPath + "\\src\\" + input_ClassName + ".h", 'w') as target_header:
    target_header.write(header_template.format(**HeaderVariables))

# CPP
with open("template_project_source.cpp", 'r') as source_cpp:
    cpp_template = source_cpp.read()

with open(input_TargetPath + "\\src\\" + input_ClassName + ".cpp", 'w') as target_cpp:
    target_cpp.write(cpp_template.format(**SourceVariables))


# Main CPP
with open(input_TargetPath + "\\src\\main.cpp", 'w') as main_cpp:
    source = ""
    source += "#pragma comment(linker,\"/SUBSYSTEM:windows /ENTRY:mainCRTStartup\")\n"
    source += "#include \"" + input_ClassName + ".h\"" + "\n\n"
    source += "int main()\n"
    source += "{\n"
    source += "mc::UniqueRef<" + input_ClassName + "> app = mc::MakeUniqueRef<" + input_ClassName + ">();\n"
    source += "app->Setup();\n"
    source += "app->Run();\n\n"
    source += "return 0;\n"
    source += "}\n"

    main_cpp.write(source)

# CMakeLists
with open(input_TargetPath + "\\CMakeLists.txt", 'w') as cmakelists:
    project_name = "".join(input_ProjectName.split())
    source = "cmake_minimum_required(VERSION 3.0)\n"
    source += "set(CMAKE_CONFIGURATION_TYPES \"Debug;Release;MinSizeRel;RelWithDebInfo\")\n"
    source += "set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} /std:c++17 /EHsc\")\n"
    source += "\n"
    source += "add_definitions(-DUNICODE -D_UNICODE)\n"
    source += "\n"
    source += "project(" + project_name + ")\n"
    source += "\n"
    source += "set(HEADERS\n"
    source += "src/" + input_ClassName + ".h\n"
    source += ")\n"
    source += "\n"
    source += "set(SOURCES\n"
    source += "src/" + input_ClassName + ".cpp\n"
    source += "src/main.cpp\n"
    source += ")\n"
    source += "\n"
    source += "add_executable(\n"
    source += project_name + "\n"
    source += "${HEADERS}\n"
    source += "${SOURCES}\n"
    source += ")\n"
    source += "\n"

    cmakelists.write(source)

build_dir = input_TargetPath + "\\build"
if not os.path.exists(build_dir):
    os.makedirs(build_dir)

os.chdir(build_dir)
os.system("cmake ..")
