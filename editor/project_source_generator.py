import os
import sys
from mclayout_reader import *

input_TargetPath    = sys.argv[1]
input_ProjectName   = sys.argv[2]
input_ClassName     = sys.argv[3]
input_MCSourcePath  = sys.argv[4].replace('\\', '/')
input_LibDbgPath    = sys.argv[5].replace('\\', '/')
input_LibRelPath    = sys.argv[6].replace('\\', '/')

print(input_MCSourcePath)
print(input_LibDbgPath)
print(input_LibRelPath)

reader = MCLayoutReader(input_TargetPath + "\\" + input_ProjectName + ".mc", input_ClassName)
reader.generate_cpp_source()

HeaderVariables = {}
HeaderVariables['ClassName'] = input_ClassName
HeaderVariables['PublicDataMembers'] = reader.get_public_data_members_source_string()
HeaderVariables['PrivateDataMembers'] = reader.get_private_data_members_source_string()
HeaderVariables['PublicMemberFunctions'] = reader.get_public_member_function_declarations_source_string()
HeaderVariables['PrivateMemberFunctions'] = reader.get_private_member_function_declarations_source_string()

SourceVariables = {}
SourceVariables['ClassName'] = input_ClassName
SourceVariables['UserInterfaceSourceCode'] = reader.get_cpp_source_string()
SourceVariables['WindowCreationSourceCode'] = reader.get_window_creation_source_string()
SourceVariables['PublicMemberFunctionDefinitions'] = reader.get_public_member_function_definitions_source_string()
SourceVariables['PrivateMemberFunctionDefinitions'] = reader.get_private_member_function_definitions_source_string()

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
    source = """
    cmake_minimum_required(VERSION 3.0)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
    set(CMAKE_CXX_FLAGS "${{CMAKE_CXX_FLAGS}} /std:c++17 /EHsc")

    add_definitions(-DUNICODE -D_UNICODE)

    project({0})

    set(HEADERS
        src/{1}.h
    )

    set(SOURCES
        src/{1}.cpp
        src/main.cpp
    )

    include_directories(
        {2}
    )

    add_executable(
        ${{PROJECT_NAME}}
        ${{HEADERS}}
        ${{SOURCES}}
    )

    find_library(
        MONOCHROME_LIB_DEBUG
        NAMES Monochrome
        HINTS {3}
    )

    find_library(
    MONOCHROME_LIB_RELEASE
    NAMES Monochrome
    HINTS {4}
    )

    target_link_libraries(${{PROJECT_NAME}} debug ${{MONOCHROME_LIB_DEBUG}})
    target_link_libraries(${{PROJECT_NAME}} optimized ${{MONOCHROME_LIB_RELEASE}})
    """.format(project_name, input_ClassName, input_MCSourcePath, input_LibDbgPath, input_LibRelPath)

    cmakelists.write(source)

build_dir = input_TargetPath + "\\build"
if not os.path.exists(build_dir):
    os.makedirs(build_dir)

os.chdir(build_dir)
os.system("cmake ..")
