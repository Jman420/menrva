# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.8)

# Set Compiler Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(LIBS_ROOT_DIR ${CMAKE_SOURCE_DIR}/../../../../../libs)
set(KFR_OUTPUT_DIR ${LIBS_ROOT_DIR}/kfr/out)
set(KFR_LIB_DIR ${KFR_OUTPUT_DIR}/android/${ANDROID_ABI})
set(PROTOBUF_OUTPUT_DIR ${LIBS_ROOT_DIR}/protobuf/out)
set(PROTOBUF_LIB_DIR ${PROTOBUF_OUTPUT_DIR}/android/${ANDROID_ABI})
set(TEST_SRC_DIR ${CMAKE_SOURCE_DIR}/../../androidTest/cpp)
set(LIBRARY_EXT a)

find_library(
        log-lib
        log
)

# Add KFR Dependency
add_library(
        libkfr-dft
        STATIC IMPORTED
)
set_target_properties(
        libkfr-dft
        PROPERTIES IMPORTED_LOCATION
        ${KFR_LIB_DIR}/libkfr_dft.${LIBRARY_EXT}
)

# Add Protobuf Depenency
add_library(
        libprotobuf-lite
        STATIC IMPORTED
)
set_target_properties(
        libprotobuf-lite
        PROPERTIES IMPORTED_LOCATION
        ${PROTOBUF_LIB_DIR}/libprotobuf-lite.${LIBRARY_EXT}
)

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
aux_source_directory(
        hosts/android/command_handlers
        HOST_COMMAND_HANDLER_SOURCES
)
add_library(
        MenrvaEngine
        SHARED

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

        #----------- Android Host Specific Implementations -----------
        hosts/android/log/AndroidLogger.cpp
        hosts/android/tools/AndroidCommandIdCalculator.cpp
        hosts/android/tools/AndroidServiceLocator.cpp

        hosts/android/commands/AndroidHost_Generic_Command.cpp

        ${HOST_COMMAND_HANDLER_SOURCES}

        hosts/android/AndroidInterface.cpp

        hosts/android/jni/menrva_activities_JniInterface.cpp
        hosts/android/jni/menrva_service_JniInterface.cpp
)
target_include_directories(
        MenrvaEngine
        PRIVATE
        ${PROTOBUF_OUTPUT_DIR}/include
        ${KFR_OUTPUT_DIR}/include
)
target_link_libraries(
        MenrvaEngine

        ${log-lib}
        libkfr-dft
        libprotobuf-lite
)

add_library(
        MenrvaEngineTest
        SHARED

        ${TEST_SRC_DIR}/jni/menrva_androidTest_EngineDebugger_Jni.cpp
        ${TEST_SRC_DIR}/jni/menrva_androidTest_CommandProcessorTests_Jni.cpp
        ${TEST_SRC_DIR}/jni/menrva_androidTest_EngineInterface_Jni.cpp
        ${TEST_SRC_DIR}/tools/engine_debugging.cpp
)
target_include_directories(
        MenrvaEngineTest
        PRIVATE
        ${PROTOBUF_OUTPUT_DIR}/include
        ${KFR_OUTPUT_DIR}/include
)
target_link_libraries(
        MenrvaEngineTest

        MenrvaEngine
        libprotobuf-lite
)