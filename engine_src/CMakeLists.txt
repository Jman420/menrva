# CMakeList.txt : CMake project for menrvaEngine, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

project("MenrvaEngine")

# Set Compiler Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# External Folder Locations
set(LIBS_ROOT_DIR ${CMAKE_SOURCE_DIR}/../libs)
set(KFR_SOURCE_DIR ${LIBS_ROOT_DIR}/kfr)
set(KFR_BINARY_DIR ${KFR_SOURCE_DIR}/out)
set(PROTOBUF_OUTPUT_DIR ${LIBS_ROOT_DIR}/protobuf/out)

set(PROTOBUF_LIB_DIR ${PROTOBUF_OUTPUT_DIR}/host)
if (ANDROID)
    set(PROTOBUF_LIB_DIR ${PROTOBUF_OUTPUT_DIR}/android/${ANDROID_ABI})
endif()

set(LIBRARY_EXT lib)
if (ANDROID)
    set(LIBRARY_EXT a)
endif()

# Add KFR Dependency
add_library(
    libkfr-dft
    STATIC IMPORTED
)
set_target_properties(
    libkfr-dft
    PROPERTIES IMPORTED_LOCATION
    ${KFR_BINARY_DIR}/kfr_dft.${LIBRARY_EXT}
)

# Add Protobuf Depenency
add_library(
    libprotobuf-lite
    STATIC IMPORTED
)
set_target_properties(
    libprotobuf-lite
    PROPERTIES IMPORTED_LOCATION
    ${PROTOBUF_LIB_DIR}/libprotobuf-lited.${LIBRARY_EXT}
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

# Define Library Source Files
add_library(
    MenrvaEngine
    STATIC

    log/LogWriterBase.cpp
    log/LogProducer.cpp

    tools/StringOperations.cpp
    tools/MathOperations.cpp
    tools/Buffer.cpp
    tools/ConversionBuffer.cpp

    audio/AudioBuffer.cpp
    audio/AudioIOBufferBase.cpp
    audio/AudioInputBuffer.cpp
    audio/AudioOutputBuffer.cpp
    audio/AudioComponentsBuffer.cpp

    ir/WaveGenerator.cpp
    ir/FirGenerator.cpp

    fft/FftInterfaceBase.cpp
    fft/KfrInterface.cpp

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
)
target_include_directories(
    MenrvaEngine
    PRIVATE
    ${PROTOBUF_OUTPUT_DIR}/include
    ${KFR_SOURCE_DIR}/include
)
target_link_libraries(
    MenrvaEngine

    libkfr-dft
    libprotobuf-lite
)
