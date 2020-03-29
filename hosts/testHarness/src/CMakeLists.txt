# CMakeList.txt : CMake project for MenrvaTestHarness, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

project("MenrvaTestHarness")

set(REPO_ROOT_DIR ${CMAKE_SOURCE_DIR}/../../..)
set(MENRVA_ENGINE_ROOT_DIR ${REPO_ROOT_DIR}/engine)
set(MENRVA_ENGINE_SRC_DIR ${MENRVA_ENGINE_ROOT_DIR}/src)
set(MENRVA_ENGINE_OUTPUT_DIR ${MENRVA_ENGINE_ROOT_DIR}/out/host/${CMAKE_BUILD_TYPE})

add_subdirectory(
    ${MENRVA_ENGINE_SRC_DIR}
    ${MENRVA_ENGINE_OUTPUT_DIR}
)

find_package(
    wxWidgets
    REQUIRED
    
    core
    base
)
include(${wxWidgets_USE_FILE})

file(
    GLOB_RECURSE
    HEADER_FILES
    *.h
)
set_source_files_properties(
    ${HEADER_FILES}
    PROPERTIES
    IS_HEADER_ONLY ON
)

add_executable(
    ${PROJECT_NAME}
    WIN32

    "tools/TextCtrlLogger.cpp"

    "TestHarnessApp.cpp"
    "TestHarnessWindow.cpp"

    ${HEADER_FILES}
)

target_link_libraries(
    ${PROJECT_NAME}
        
    MenrvaEngine
    ${wxWidgets_LIBRARIES}
)
