# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(REPO_ROOT_DIR ${CMAKE_SOURCE_DIR}/../../../../..)
set(FFTW_OUTPUT_DIR ${REPO_ROOT_DIR}/fftw/out)
set(KISSFFT_OUTPUT_DIR ${REPO_ROOT_DIR}/kissfft/out)
set(KFR_SOURCE_DIR ${REPO_ROOT_DIR}/kfr)
set(KFR_BINARY_DIR ${KFR_SOURCE_DIR}/out)

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

add_subdirectory(
        ${KFR_SOURCE_DIR}
        ${KFR_BINARY_DIR}
)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        MenrvaEngine

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        abstracts/logger_base.cpp
        abstracts/logging_base.cpp
        abstracts/fft_interface_base.cpp
        abstracts/effect_base.cpp
        abstracts/convolution_operations_base.cpp
        abstracts/audio_io_buffer_base.cpp

        tools/android_logger.cpp
        tools/service_locator.cpp
        tools/math_operations.cpp
        tools/buffer.cpp
        tools/conversion_buffer.cpp

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

        effects/bass_boost.cpp
        effects/equalizer.cpp
        effects/stereo_widener.cpp

        engine/engine_interface.cpp
        engine/effects_engine.cpp
        engine/command_map.cpp

        module_interface.cpp

        jni/com_monkeystable_menrva_activities_JniInterface.cpp
)

target_include_directories(
        MenrvaEngine
        PRIVATE
        ${FFTW_OUTPUT_DIR}/include
        ${KISSFFT_OUTPUT_DIR}/include
)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log
)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        MenrvaEngine

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        libfftw
        libkissfft
        kfr
        kfr_dft
)