# Contributing to Menrva

## Project Architecture
The Menrva Project consists of various subprojects used to encapsulate portions of the Project by functionality.  At the core of the Project is the MenrvaEngine which is implemented as a C++ CMake Project for inclusion as a CMake Subproject in Host Implementations.  Each Host Implementation contains the neccessary logic to integrate MenrvaEngine into a Host Environment such as Windows, Linux, MacOS, Android, iOS, etc.

## Versioning
Menrva uses [Semantic Versioning](https://semver.org/) for easy consumption in other projects if needed.  Please adhere to this versioning pattern when making contributions.  See AOSP's [Version your app](https://developer.android.com/studio/publish/versioning) guide for details on updating the Android App versioning.  Each subproject in the Menrva Project can have its own version for clarity and bug fixing.

## Engine Development
The MenrvaEngine represents the heart of the Menrva Project.  The Engine handles all of the audio processing functionality and is designed to be shared across platforms.  This subproject is modified when adding new audio effects, DSP algorithms or engine commands.  Detailed Engine Architecture can be found [here](EngineArchitecture.md).

### Development Requirements
  - C++ IDE with CMake Support (recommendation : Visual Studio)
  - Powershell

### Dependencies
  - [KFRLib](https://www.kfrlib.com/)
  - [Protobuf](https://developers.google.com/protocol-buffers)
  
### Building Dependencies
MenrvaEngine dependencies are included as submodules containing build scripts for the dependency.  To decrease build times these dependencies are precompiled rather than including them as part of the Menrva Engine build process.  Specific build instructions for each dependency can be found in the dependency's submodule.  All Project Dependencies are stored in the [/libs]](../libs) folder.

### Generating Templated Engine Source Code
MenrvaEngine uses Templated Source Code for some portions of it's codebase for portability and simplifying certain development processes.  Scripts have also been provided to automate using and populating the Template Source Code files when necessary.  Source Code Files which are generated from Templates will carry a header comment identifying it as generated source code and provide the path to the Source Code Template file.  These files are not tracked by source control and are explicitly ignored in the root gitignore.

#### Steps
  - Use [/engine/scripts/generate_protobuf_commands.ps1](../engine/scripts/generate_protobuf_commands.ps1) to generate the necessary C++ Source Code from the Protobuf Command Files
  - Use [/engine/scripts/generate_command_handler_base.ps1](../engine/scripts/generate_command_handler_base.ps1) to generate the TypedCommandHandlerBase Class Source Files
  - Use [/engine/scripts/generate_command_handler_map.ps1](../engine/scripts/generate_command_handler_map.ps1) to generate the CommandHandlerMap Class Source Files

### Building the Engine
Once the above steps in [Generating Templated Engine Source Code](#generating-templated-engine-source-code) have been completed the MenrvaEngine Project is ready to be built.  There are many ways that the CMake Project can be compiled and won't be covered here.  Visual Studio is recommended using the CMake Project support; when using Visual Studio the Project can be built as any other Visual Studio Project.

After any successful Engine Build the [copy_include_headers.ps1](../engine/scripts/copy_include_headers.ps1) script should be executed to ensure that the Include Headers are kept up to date with the Binaries.  This is required for integrating MenrvaEngine into any other Projects including Host Implementations.

### Adding New Engine Commands
MenrvaEngine uses a Command-based API powered by [Protobuf](https://developers.google.com/protocol-buffers).  This allows for easily adding new features and functionality to the Engine with a decoupled mechanism for inter-process communications.  Scripts and templates are provided to simplify the processes of creating new Protobuf Command Files,  generating source code files from Protobuf Command Files and generating new command handler source code.

#### Steps
  - Use [/engine/scripts/create_new_protobuf_command.ps1](../engine/scripts/create_new_protobuf_command.ps1) to create a new Protobuf Command File
  - Edit the newly created Protobuf Command File's Request & Response structures with any necessary data points
  - Use [/engine/scripts/generate_protobuf_commands.ps1](../engine/scripts/generate_protobuf_commands.ps1) to generate the necessary C++ Source Code from the Protobuf Command Files
  - Use [/engine/scripts/create_new_command_handler.ps1](../engine/scripts/create_new_command_handler.ps1) to generate a new Command Handler for the newly created Command
  - Add logic to the newly created Command Handler's Source Code File in [/engine/src/command_handlers](../engine/src/command_handlers)

### Configuring Calculation Precision
See [Calculation Precision](../README.md#calculation-precision) for details about Calculation Precision.

#### Enabling Double Precision
  - Uncomment the ```//#define MENRVA_DOUBLE_PRECISION``` line in [config.h](menrva/app/src/main/cpp/config.h)

## Host Development
On it's own the MenrvaEngine is fairly useless; although it contains powerful algorithms and effects, it still needs to be provided input for processing.  Menrva Host Implementations encapsulate any data structures and logic required to integrate MenrvaEngine into a Host Environment to provide the inputs that the MenrvaEngine needs for processing.  A Host Environment can be any application or library which includes the MenrvaEngine Static Library.  Menrva Host Implementations have two options for interacting with it's MenrvaEngine : 
  - Protobuf-based Command API : recommended for inter-process and inter-language support
  - MenrvaEngine's Public API : recommended for when embedded in application

*NOTE:* Host Implementations are expected to define implementations for the headers contained in [/engine/src/host](../engine/src/host).

### Test Harness
***Under Construction***

### Android
Provides support for integrating MenrvaEngine into Android's AudioFlinger and an App for interacting with the Engine.

#### Development Requirements
  - Android Studio
  - Android SDK
  - Android NDK
  - Powershell

#### Generating Templated Android Source Code
To support MenvaEngine's Protobuf Command API the Android Implementation uses Templated Source Code to ensure compatible command structures.  Scripts have also been provided to automate using and populating the Template Source Code files when necessary.  Source Code Files which are generated from Templates will carry a header comment identifying it as generated source code and provide the path to the Source Code Template file.  These files are not tracked by source control and are explicitly ignored in the root gitignore.

##### Steps
  - Use [/hosts/android/scripts/generate_protobuf_commands.ps1](/hosts/android/scripts/generate_protobuf_commands.ps1) to generate the necessary Java Source Code from the Protobuf Command Files

#### Building the Android Implementation
Once the above steps in [Generating Templated Android Source Code](#generating-templated-android-source-code) have been completed the Android Implementation is ready to be built.  The Android Implementation provides an AndroidStudio Project for IDE supported builds as well as a Powershell script for automated builds.  Only the Powershell automated build will be covered here.

*NOTE:* The Android Implementation does not require MenrvaEngine to be pre-compiled.  Only its Generated Source Code must be provided.

##### Steps
  - Generate MenrvaEngine Templated Source Code as described in [Generating Templated Engine Source Code](#generating-templated-engine-source-code)
  - Use [/engine/scripts/copy_include_headers.ps1](/engine/scripts/copy_include_headers.ps1) to gopy the MenrvaEngine Include Headers to its Output Directory
  - Generate Android Templated Source Code as described in [Generating Templated Android Source Code](#generating-templated-android-source-code)
  - Use [/hosts/android/scripts/build.ps1](/hosts/android/scripts/build.ps1) to execute a Gradle build

#### Generating Magisk Module
  - Perform a successful build as described in [Building the Android Implementation](#building-the-android-implementation)
  - Use [/hosts/android/scripts/prepare_artifacts.ps1](/hosts/android/scripts/prepare_artifacts.ps1) to prepare the build artifacts
  - Use [/hosts/android/scripts/prepare_magisk_module_template.ps1](/hosts/android/scripts/prepare_magisk_module_template.ps1) to download and extract the MMT-EX Template
  - Use [/hosts/android/scripts/generate_magisk_module.ps1](/hosts/android/scripts/generate_magisk_module.ps1) to generate the Magisk Module Zip
  - Magisk Module Zip will be in /hosts/android/artifacts/ directory

#### Scripted Installation
  - Perform a successful build as described in [Building the Android Implementation](#building-the-android-implementation)
  - Use [/hosts/android/scripts/prepare_artifacts.ps1](/hosts/android/scripts/prepare_artifacts.ps1) to prepare the build artifacts
  - Run an Android Emulated Device via AVD -OR- Attach an Android Device via USB with ADB Debugging Enabled
  - Use [/hosts/android/scripts/deploy_to_device.ps1](/hosts/android/scripts/deploy_to_device.ps1) to install the build via ADB (multiple executions may be necessary for ADB to initialize)
