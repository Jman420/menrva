# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(LIBS_ROOT_DIR ${CMAKE_SOURCE_DIR}/../../../../../libs)
set(KFR_SOURCE_DIR ${LIBS_ROOT_DIR}/kfr)
set(KFR_BINARY_DIR ${KFR_SOURCE_DIR}/out)
set(FFTW_OUTPUT_DIR ${LIBS_ROOT_DIR}/fftw/out)
set(KISSFFT_OUTPUT_DIR ${LIBS_ROOT_DIR}/kissfft/out)
set(PROTOBUF_OUTPUT_DIR ${LIBS_ROOT_DIR}/protobuf/out)
set(TEST_SRC_DIR ${CMAKE_SOURCE_DIR}/../../androidTest/cpp)

find_library(
        log-lib
        log
)

add_subdirectory(
        ${KFR_SOURCE_DIR}
        ${KFR_BINARY_DIR}
)

add_library(
        libfftw
        SHARED IMPORTED
)
set_target_properties(
        libfftw
        PROPERTIES IMPORTED_LOCATION
        ${FFTW_OUTPUT_DIR}/${ANDROID_ABI}/libfftw3.so
)

add_library(
        libkissfft
        SHARED IMPORTED
)
set_target_properties(
        libkissfft
        PROPERTIES IMPORTED_LOCATION
        ${KISSFFT_OUTPUT_DIR}/${ANDROID_ABI}/libkissfft.so
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
add_library(
        MenrvaEngine
        SHARED

        abstracts/logger_base.cpp
        abstracts/logging_base.cpp
        abstracts/fft_interface_base.cpp
        abstracts/effect_base.cpp
        abstracts/convolution_operations_base.cpp
        abstracts/audio_io_buffer_base.cpp
        abstracts/single_channel_effect_base.cpp
        abstracts/multi_channel_effect_base.cpp

        tools/android_logger.cpp
        tools/service_locator.cpp
        tools/math_operations.cpp
        tools/buffer.cpp
        tools/conversion_buffer.cpp
        tools/wave_generator.cpp

        audio/audio_buffer.cpp
        audio/audio_input_buffer.cpp
        audio/audio_output_buffer.cpp
        audio/audio_components_buffer.cpp

        ir/fir_generator.cpp

        fft/fftw_interface.cpp
        fft/kissfft_interface.cpp
        fft/kfr_interface.cpp

        convolver/convolution_operations.cpp
        convolver/convolver.cpp

        effects/master_limiter.cpp
        effects/bass_boost.cpp
        effects/equalizer.cpp
        effects/stereo_widener.cpp
        effects/single_channel_effects_bundle.cpp
        effects/multi_channel_effects_bundle.cpp

        engine/engine_interface.cpp
        engine/effects_engine.cpp
        engine/command_map.cpp

        module_interface.cpp

        jni/menrva_activities_JniInterface.cpp
        jni/menrva_service_JniInterface.cpp

        ${MESSAGES_SOURCES}
)
target_include_directories(
        MenrvaEngine
        PRIVATE
        ${FFTW_OUTPUT_DIR}/include
        ${KISSFFT_OUTPUT_DIR}/include
        ${PROTOBUF_OUTPUT_DIR}/include
)
target_link_libraries(
        MenrvaEngine

        ${log-lib}
        libfftw
        libkissfft
        kfr
        kfr_dft
        libprotobuf-lite
)

add_library(
        MenrvaEngineTest
        SHARED

        ${TEST_SRC_DIR}/jni/menrva_androidTest_JniInterface.cpp
        ${TEST_SRC_DIR}/tools/engine_debugging.cpp
)
target_link_libraries(
        MenrvaEngineTest

        MenrvaEngine
)