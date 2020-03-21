# Menrva Engine
The MenrvaEngine is the core of the Menrva Project.  It provides all of the audio effect & digital signal processing (DSP) functionality which allows Users to tune their audio.  It also provides a Command-based API using Protobuf to support inter-process communications in situations when the Engine is embedded in a separate process than the process for configuring it.

## Contributing
See [CONTRIBUTING](../docs/CONTRIBUTING.md) document for details on building and contributing to the Menrva Projects.

See [EngineArchitecture](docs/EngineArchitecture.md) document for details on the Engine Project Architecture.

## Adding Menrva Engine to Projects
The MenrvaEngine will often need to be extended to support the environment which is hosting it.  This can be accomplished by adding the MenrvaEngine to another Project and providing any missing structures and/or functions.

### CMake Subproject
The easiest way to add MenrvaEngine to another Project is by adding it as a CMake Subproject.  To support being included from various file locations it uses the 'REPO_ROOT_DIR' variable to reference the expected Repository Root directory.  An example of this can be found below :

```
set(REPO_ROOT_DIR ${CMAKE_SOURCE_DIR}/../../../../../../..)
set(MENRVA_ENGINE_ROOT_DIR ${REPO_ROOT_DIR}/engine)
set(MENRVA_ENGINE_SRC_DIR ${MENRVA_ENGINE_ROOT_DIR}/src)
set(MENRVA_ENGINE_OUTPUT_DIR ${MENRVA_ENGINE_ROOT_DIR}/out/android)

...

add_subdirectory(
        ${MENRVA_ENGINE_SRC_DIR}
        ${MENRVA_ENGINE_OUTPUT_DIR}
)

...

target_link_libraries(
        ${PROJECT_NAME}

        MenrvaEngine
)
```

### Prebuilding Library
To reduce build times the MenrvaEngine can also be precompiled and imported as a Static Library.  This process will not be covered here since there are many ways to build the project.  A couple of options are to use CMake & Clang or to use CMake to generate an IDE Project and perform the build there.

### Exporting Include Headers
Regardless of how MenrvaEngine is included in a Project its Include Headers will need to be exported to the expected location.  This can be accomplished by simply executing [/engine/scripts/copy_include_headers.ps1](scripts/copy_include_headers.ps1) and is required to be executed after any modifications to MenrvaEngine Header files.

## Interacting with Menrva Engine
Without any interaction the MenrvaEngine will not actually do anything since, by default, all of its effects are disabled.  Once an instance of MenrvaEngine is created it must be configured and controlled externally.  To facilitate these interactions there are two forms of API for MenrvaEngine for different circumstances.

### Command-based API
A Command-based API leveraging Protobuf is provided to support inter-process communications.  This is the recommended form of API when the MenrvaEngine instance and the controlling mechanism (UI, console or frontend of some kind) are in separate processes.  The Command-based API can always be referenced from the [/engine/protobuf](protobuf) directory.

Depending on the language and environment being used to implement the Engine's controlling mechanism the syntax for instantiating and sending a command to an instance of MenrvaEngine will change.  An example of this kind of API Interaction can be found in the [Android Implementation](../hosts/android) spread between the [AudioEffectInterface](../hosts/android/src/app/src/main/java/com/monkeystable/menrva/utilities/AudioEffectInterface.java) & [CommandBase](../hosts/android/src/app/src/main/java/com/monkeystable/menrva/abstracts/CommandBase.java) classes.

### Direct API
In one form or another most Host Implementations will need to use the Direct API.  MenrvaEngine's Direct API can always be referenced from the '/engine/out/include/menrvaEngine' directory after executing the [/engine/scripts/copy_include_headers.ps1](../engine/scripts/copy_include_headers.ps1) script.

The most common use of the Direct API will be instantiating any necessary MenrvaEngine Objects and piping data between the Host Environment and MenrvaEngine.  An example of this kind of API Interaction can be found in the [Android Implementation](../hosts/android) in the [/hosts/android/src/app/src/main/cpp/AndroidInterface.cpp](../hosts/android/src/app/src/main/cpp/AndroidInterface.cpp) class.

## Calculation Precision
By default MenrvaEngine performs single precision calculations using the C++ float_32 type.  This minimizes the memory & cpu footprint for the Engine, but it can also support double precision calculations if desired.  Enabling double precision will cause MenrvaEngine to use the C++ double type to perform all calculations.  This can result in less distortion in the resulting audio, but is usually impreceptible to most listeners.

#### Enabling Double Precision
  - Uncomment the ```//#define MENRVA_DOUBLE_PRECISION``` line in [BuildConfig.h](src/BuildConfig.h)
  - Rebuild MenrvaEngine
