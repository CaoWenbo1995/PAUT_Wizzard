#cmake version
cmake_minimum_required(VERSION 3.1.0)

# The version number.
set(PAUT_CORE_TEST_VERSION_MAJOR 1)
set(PAUT_CORE_TEST_VERSION_MINOR 0)

#project name
project(PAUT_WIZZARD)

#include qt and opengl
find_package(Qt5Widgets REQUIRED)

#add macros
add_definitions(-DAOS_VERSION)

#file definition
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#main
set(PROJECT_MAIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" CACHE PATH "MAIN directory")
file(GLOB project_main_head_files  ${PROJECT_MAIN_DIR}/*.h)
file(GLOB project_main_source_files  ${PROJECT_MAIN_DIR}/*.cpp)
source_group("Header Files" FILES ${project_main_head_files})
source_group("Source Files" FILES ${project_main_source_files})

#ui
set(PROJECT_UI_DIR "${PROJECT_MAIN_DIR}/ui" CACHE PATH "UI directory")
file(GLOB project_ui_header_files  ${PROJECT_UI_DIR}/*.h)
file(GLOB project_ui_source_files  ${PROJECT_UI_DIR}/*.cpp)
source_group("Header Files\\ui" FILES ${project_ui_header_files})
source_group("Source Files\\ui" FILES ${project_ui_source_files})

#moc qt gui
QT5_WRAP_CPP(project_gui_moc_source_files ${project_ui_header_files})

#moc resource
set(PROJECT_RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resource" CACHE PATH "Resource directory")
file(GLOB project_resource_qrc_files ${PROJECT_RESOURCE_DIR}/*.qrc)
QT5_ADD_RESOURCES(qrc_complied ${project_resource_qrc_files})

source_group("Generated Files" FILES ${project_gui_moc_source_files} ${qrc_complied})
source_group("Resource Files" FILES ${project_resource_qrc_files})

include_directories(${PROJECT_UI_DIR}
                    ${PROJECT_MAIN_DIR})

add_executable(${PROJECT_NAME}
               ${qrc_complied}
               ${project_resource_rc_files}
               ${project_main_source_files}
               ${project_ui_header_files}
               ${project_ui_source_files}
               ${project_gui_moc_source_files})

#bin path
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

#lib
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)