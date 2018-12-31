# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.
cmake_minimum_required(VERSION 3.4.1)

set(REPO_ROOT_DIR ${CMAKE_SOURCE_DIR}/../..)

add_library(
        libfftw3
        SHARED IMPORTED
)
set_target_properties(
        libfftw3
        PROPERTIES IMPORTED_LOCATION
        ${REPO_ROOT_DIR}/fftw3/out/${ANDROID_ABI}/libfftw3.so
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
        src/main/cpp/abstracts/logger_base.cpp
        src/main/cpp/abstracts/logging_base.cpp
        src/main/cpp/abstracts/fft_interface_base.cpp

        src/main/cpp/tools/android_logger.cpp
        src/main/cpp/tools/buffer.cpp
        src/main/cpp/tools/service_locator.cpp
        src/main/cpp/tools/math_operations.cpp
        
        src/main/cpp/audio/audio_components_buffer.cpp
        
        src/main/cpp/fft/fftw_interface.cpp

        src/main/cpp/convolver/convolution_operations.cpp
        src/main/cpp/convolver/convolver.cpp

        src/main/cpp/audio/audio_components_buffer.cpp
        src/main/cpp/audio/fir_generator.cpp

        src/main/cpp/fft/fftw_interface.cpp

        src/main/cpp/convolver/convolution_operations.cpp

        src/main/cpp/effects/bass_boost.cpp
        src/main/cpp/effects/equalizer.cpp
        src/main/cpp/effects/stereo_widener.cpp

        src/main/cpp/engine/engine_interface.cpp
        src/main/cpp/engine/effects_engine.cpp
        src/main/cpp/engine/command_map.cpp
        
        src/main/cpp/module_interface.cpp

        src/main/cpp/jni/com_monkeystable_menrva_activities_JniInterface.cpp
)

target_include_directories(
        MenrvaEngine
        PRIVATE
        ${REPO_ROOT_DIR}/fftw3/out/include
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
        libfftw3
)
