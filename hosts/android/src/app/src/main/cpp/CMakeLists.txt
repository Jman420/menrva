# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.
cmake_minimum_required(VERSION 3.8)

project(MenrvaEngine-Android)

# Set Compiler Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(TEST_SRC_DIR ${CMAKE_SOURCE_DIR}/../../androidTest/cpp)
set(REPO_ROOT_DIR ${CMAKE_SOURCE_DIR}/../../../../../../..)
set(MENRVA_ENGINE_ROOT_DIR ${REPO_ROOT_DIR}/engine)
set(MENRVA_ENGINE_SRC_DIR ${MENRVA_ENGINE_ROOT_DIR}/src)
set(MENRVA_ENGINE_OUTPUT_DIR ${MENRVA_ENGINE_ROOT_DIR}/out/android/${ANDROID_ABI})

add_subdirectory(
        ${MENRVA_ENGINE_SRC_DIR}
        ${MENRVA_ENGINE_OUTPUT_DIR}
)

find_library(
        log-lib
        log
)

aux_source_directory(
        command_handlers
        HOST_COMMAND_HANDLER_SOURCES
)
add_library(
        ${PROJECT_NAME}
        SHARED

        log/AndroidLogger.cpp
        tools/AndroidCommandIdCalculator.cpp
        tools/AndroidServiceLocator.cpp

        commands/AndroidHost_Generic_Command.cpp

        ${HOST_COMMAND_HANDLER_SOURCES}

        AndroidInterface.cpp

        jni/menrva_activities_JniInterface.cpp
        jni/menrva_service_JniInterface.cpp
)
target_link_libraries(
        ${PROJECT_NAME}

        ${log-lib}
        MenrvaEngine
)

add_library(
        MenrvaEngineTest
        SHARED

        ${TEST_SRC_DIR}/jni/menrva_androidTest_EngineDebugger_Jni.cpp
        ${TEST_SRC_DIR}/jni/menrva_androidTest_CommandProcessorTests_Jni.cpp
        ${TEST_SRC_DIR}/jni/menrva_androidTest_EngineInterface_Jni.cpp
        ${TEST_SRC_DIR}/tools/engine_debugging.cpp
)
target_link_libraries(
        MenrvaEngineTest

        ${PROJECT_NAME}
)
