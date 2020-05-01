# CMakeList.txt : CMake project for menrvaEngine, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

project(MenrvaEngine)

# Set Compiler Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Android Hacks
set(KFR_LIB_PREFIX)
if (ANDROID)
    set(KFR_LIB_PREFIX lib)
endif()
message(STATUS "KFR_LIB_PREFIX : ${KFR_LIB_PREFIX}")
set(LIBRARY_EXT lib)
if (ANDROID)
    set(LIBRARY_EXT a)
endif()
message(STATUS "LIBRARY_EXT : ${LIBRARY_EXT}")

# Debug Hacks
set(PROTOBUF_LIB_SUFFIX)
if (CMAKE_BUILD_TYPE STREQUAL Debug AND NOT ANDROID)
    set(PROTOBUF_LIB_SUFFIX d)
endif()
message(STATUS "PROTOBUF_LIB_SUFFIX : ${PROTOBUF_LIB_SUFFIX}")

# Folder Locations
if (NOT DEFINED REPO_ROOT_DIR)
    set(REPO_ROOT_DIR ${CMAKE_SOURCE_DIR}/../..)
endif()
message(STATUS "REPO_ROOT_DIR : ${REPO_ROOT_DIR}")

set(ENGINE_OUTPUT_DIR ${REPO_ROOT_DIR}/engine/out/host/${CMAKE_BUILD_TYPE})
if (ANDROID)
    set(ENGINE_OUTPUT_DIR ${REPO_ROOT_DIR}/engine/out/android/${ANDROID_ABI})
endif()
set(ENGINE_INCLUDE_DIR ${ENGINE_OUTPUT_DIR}/../../include)
message(STATUS "ENGINE_OUTPUT_DIR : ${ENGINE_OUTPUT_DIR}")
message(STATUS "ENGINE_OUTPUT_DIR : ${ENGINE_INCLUDE_DIR}")

set(LIBS_ROOT_DIR ${REPO_ROOT_DIR}/libs)
message(STATUS "LIBS_ROOT_DIR : ${LIBS_ROOT_DIR}")

set(BOOST_DI_LIB_DIR ${LIBS_ROOT_DIR}/boostDI)

if (NOT DEFINED KFR_ROOT_DIR)
    set(KFR_ROOT_DIR ${LIBS_ROOT_DIR}/kfr)
endif()
set(KFR_OUTPUT_DIR ${KFR_ROOT_DIR}/out)
set(KFR_INSTALL_DIR ${KFR_OUTPUT_DIR}/host/${CMAKE_BUILD_TYPE})
if (ANDROID)
    set(KFR_INSTALL_DIR ${KFR_OUTPUT_DIR}/android/${ANDROID_ABI})
endif()
set(KFR_INCLUDE_DIR ${KFR_INSTALL_DIR}/include)
set(KFR_CMAKE_DIR ${KFR_INSTALL_DIR}/cmake)
message(STATUS "KFR_ROOT_DIR : ${KFR_ROOT_DIR}")
message(STATUS "KFR_OUTPUT_DIR : ${KFR_OUTPUT_DIR}")
message(STATUS "KFR_INSTALL_DIR : ${KFR_INSTALL_DIR}")
message(STATUS "KFR_INCLUDE_DIR : ${KFR_INCLUDE_DIR}")
message(STATUS "KFR_CMAKE_DIR : ${KFR_CMAKE_DIR}")

if (NOT DEFINED PROTOBUF_ROOT_DIR)
    set(PROTOBUF_ROOT_DIR ${LIBS_ROOT_DIR}/protobuf)
endif()
set(PROTOBUF_OUTPUT_DIR ${PROTOBUF_ROOT_DIR}/out)
set(PROTOBUF_INSTALL_DIR ${PROTOBUF_OUTPUT_DIR}/host/${CMAKE_BUILD_TYPE})
if (ANDROID)
    set(PROTOBUF_INSTALL_DIR ${PROTOBUF_OUTPUT_DIR}/android/${ANDROID_ABI})
endif()
message(STATUS "PROTOBUF_OUTPUT_DIR : ${PROTOBUF_OUTPUT_DIR}")
message(STATUS "PROTOBUF_INSTALL_DIR : ${PROTOBUF_INSTALL_DIR}")

# Add KFR Dependency
add_library(
    libkfr-dft
    STATIC IMPORTED
)
set_target_properties(
    libkfr-dft
    PROPERTIES IMPORTED_LOCATION
    
    ${KFR_INSTALL_DIR}/lib/${KFR_LIB_PREFIX}kfr_dft.${LIBRARY_EXT}
)

include(${KFR_CMAKE_DIR}/kfr_include.cmake)
add_library(kfr_arch_flags INTERFACE)
target_set_arch(kfr_arch_flags INTERFACE ${CPU_ARCH})
message(STATUS "CPU_ARCH : ${CPU_ARCH}")

# Add Protobuf Depenency
add_library(
    libprotobuf-lite
    STATIC IMPORTED
)
set_target_properties(
    libprotobuf-lite
    PROPERTIES IMPORTED_LOCATION

    ${PROTOBUF_INSTALL_DIR}/lib/libprotobuf-lite${PROTOBUF_LIB_SUFFIX}.${LIBRARY_EXT}
)

# Define Source Directories
aux_source_directory(
    commands/messages
    MESSAGES_SOURCES
)
aux_source_directory(
    commands
    COMMANDS_SOURCES
)
aux_source_directory(
    command_handlers
    COMMAND_HANDLER_SOURCES
)

# Define Header Files
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

# Define Library Source Files
add_library(
    ${PROJECT_NAME}
    STATIC

    "log/LogLevels.cpp"
    "log/LogOverrideManager.cpp"
    "log/ILogWriter.cpp"
    "log/ILogProducer.cpp"

    "tools/ServiceLocator.cpp"
    "tools/StringOperations.cpp"
    "tools/MathOperations.cpp"
    "tools/Buffer.cpp"
    "tools/ConversionBuffer.cpp"

    "audio/AudioIOBufferBase.cpp"
    "audio/AudioInputBuffer.cpp"
    "audio/AudioOutputBuffer.cpp"
    audio/AudioComponentsBuffer.cpp

    impulse_response/WaveGenerator.cpp
    impulse_response/FirGenerator.cpp

    fourier_transform/FftInterfaceBase.cpp
    fourier_transform/KfrInterface.cpp

    convolver/ConvolutionOperationsBase.cpp
    convolver/ConvolutionOperations.cpp
    convolver/Convolver.cpp

    effects/EffectBase.cpp
    effects/SingleChannelEffectBase.cpp
    effects/MultiChannelEffectBase.cpp
    effects/MasterLimiter.cpp
    effects/BassBoost.cpp
    effects/Equalizer.cpp
    effects/StereoWidener.cpp
    effects/SingleChannelEffectsBundle.cpp
    effects/MultiChannelEffectsBundle.cpp

    ${MESSAGES_SOURCES}
    ${COMMANDS_SOURCES}
    ${COMMAND_HANDLER_SOURCES}

    engine/EffectsEngine.cpp
    engine/CommandHandlerMap.cpp
    engine/CommandProcessor.cpp

    dependency_injection/ContainerInitializer.cpp

    ${HEADER_FILES}
)
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC

    ${KFR_INSTALL_DIR}/include
    ${PROTOBUF_INSTALL_DIR}/include
    ${BOOST_DI_LIB_DIR}/include
    ${ENGINE_INCLUDE_DIR}
)
target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC

    kfr_arch_flags
    libkfr-dft
    libprotobuf-lite
)
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES

    ARCHIVE_OUTPUT_DIRECTORY ${ENGINE_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${ENGINE_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${ENGINE_OUTPUT_DIR}
)
