# CMakeList.txt : CMake project for MenrvaTestHarness, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

project("MenrvaTestHarness")

set(REPO_ROOT_DIR ${CMAKE_SOURCE_DIR}/../../..)
set(MENRVA_ENGINE_ROOT_DIR ${REPO_ROOT_DIR}/engine)
set(MENRVA_ENGINE_SRC_DIR ${MENRVA_ENGINE_ROOT_DIR}/src)
set(MENRVA_ENGINE_OUTPUT_DIR ${MENRVA_ENGINE_ROOT_DIR}/out/host)

add_subdirectory(
    ${MENRVA_ENGINE_SRC_DIR}
    ${MENRVA_ENGINE_OUTPUT_DIR}
)

find_package(
    wxWidgets
    REQUIRED COMPONENTS
    
    net
    core
    base
)
include(${wxWidgets_USE_FILE})

add_executable(
    ${PROJECT_NAME}

    "MenrvaTestHarness.cpp"
)

target_link_libraries(
    ${PROJECT_NAME}
        
    MenrvaEngine
    ${wxWidgets_LIBRARIES}
)
