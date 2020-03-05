# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.8)

set(LIBS_ROOT_DIR ${CMAKE_SOURCE_DIR}/../../../../../libs)
set(KFR_SOURCE_DIR ${LIBS_ROOT_DIR}/kfr)
set(KFR_BINARY_DIR ${KFR_SOURCE_DIR}/out)
set(PROTOBUF_OUTPUT_DIR ${LIBS_ROOT_DIR}/protobuf/out)
set(TEST_SRC_DIR ${CMAKE_SOURCE_DIR}/../../androidTest/cpp)

find_library(
        log-lib
        log
)

set(KFR_ARCH_armeabi-v7a neon)
set(KFR_ARCH_arm64-v8a neon64)
set(KFR_ARCH_x86 avx1)
set(KFR_ARCH_x86_64 avx1)
set(CPU_ARCH ${KFR_ARCH_${ANDROID_ABI}})
add_subdirectory(
        ${KFR_SOURCE_DIR}
        ${KFR_BINARY_DIR}
)

add_library(
        libprotobuf-lite
        STATIC IMPORTED
)
set_target_properties(
        libprotobuf-lite
        PROPERTIES IMPORTED_LOCATION
        ${PROTOBUF_OUTPUT_DIR}/${ANDROID_ABI}/libprotobuf-lite.a
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

        tools/LoggerBase.cpp
        tools/LoggingBase.cpp
        tools/AndroidLogger.cpp
        tools/ServiceLocator.cpp
        tools/MathOperations.cpp
        tools/Buffer.cpp
        tools/ConversionBuffer.cpp
        tools/CommandIds.cpp
        tools/StringOperations.cpp

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

        hosts/android/commands/AndroidHost_Generic_Command.cpp

        ${HOST_COMMAND_HANDLER_SOURCES}

        hosts/android/AndroidInterface.cpp

        jni/menrva_activities_JniInterface.cpp
        jni/menrva_service_JniInterface.cpp
)
target_include_directories(
        MenrvaEngine
        PRIVATE
        ${PROTOBUF_OUTPUT_DIR}/include
)
target_link_libraries(
        MenrvaEngine

        ${log-lib}
        kfr
        kfr_dft
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
)
target_link_libraries(
        MenrvaEngineTest

        MenrvaEngine
        libprotobuf-lite
)