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

    "TestHarnessApp.cpp"
    "TestHarnessWindow.cpp"

    ${HEADER_FILES}
)

target_link_libraries(
    ${PROJECT_NAME}
        
    MenrvaEngine
    ${wxWidgets_LIBRARIES}
)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
set(CMAKE_INSTALL_DEBUG_LIBRARIES TRUE)
include(InstallRequiredSystemLibraries)
SET(APP ${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX})
SET(LIB_DIRS "${wxWidgets_LIB_DIR};${CMAKE_BINARY_DIR}/bin")
install(
    CODE "
    include(BundleUtilities)
    fixup_bundle(\"${APP}\" \"\" \"${LIB_DIRS}\")
    "
)
