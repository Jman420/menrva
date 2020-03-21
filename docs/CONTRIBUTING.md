# Contributing to Menrva

## Project Architecture
The Menrva Project consists of various subprojects used to encapsulate portions of the Project by functionality.  At the core of the Project is the MenrvaEngine which is implemented as a C++ CMake Project for inclusion as a CMake Subproject in Host Implementations.  Each Host Implementation contains the neccessary logic to integrate MenrvaEngine into a Host Environment such as Windows, Linux, MacOS, Android, iOS, etc.

See [RepoArchitecture](RepoArchitecture.md) for details on Repository File Structure & Architecture.

## Versioning
Menrva uses [Semantic Versioning](https://semver.org/) for easy consumption in other projects if needed.  Please adhere to this versioning pattern when making contributions.  See AOSP's [Version your app](https://developer.android.com/studio/publish/versioning) guide for details on updating the Android App versioning.  Each subproject in the Menrva Project can have its own version for clarity and bug fixing.

## Building Dependencies
Menrva Cross-Project dependencies are included as submodules containing build scripts for the dependency.  To decrease build times these dependencies are precompiled rather than including them as part of the Menrva Engine build process.  Specific build instructions for each dependency can be found in the dependency's submodule.  All Project Dependencies are stored in the [/libs]](../libs) folder.

## Engine Development
The MenrvaEngine represents the heart of the Menrva Project.  The Engine handles all of the audio processing functionality and is designed to be shared across platforms.  This subproject is modified when adding new audio effects, DSP algorithms or engine commands.  Detailed Engine Architecture can be found [here](EngineArchitecture.md).  Detailed Build Instructions can be found [here](../engine/docs/BuildInstructions.md).

## Host Development
On it's own the MenrvaEngine is fairly useless; although it contains powerful algorithms and effects, it still needs to be provided input for processing.  Menrva Host Implementations encapsulate any data structures and logic required to integrate MenrvaEngine into a Host Environment to provide the inputs that the MenrvaEngine needs for processing.  A Host Environment can be any application or library which includes the MenrvaEngine Static Library.  Menrva Host Implementations have two options for interacting with it's MenrvaEngine : 
  - Protobuf-based Command API : recommended for inter-process and inter-language support
  - MenrvaEngine's Public API : recommended for when embedded in application

*NOTE:*
  - Host Implementations are expected to define implementations for the headers contained in [/engine/src/host](../engine/src/host).
  - Host Implementations can be found in [/hosts](/hosts).

### Test Harness
***Under Construction***

### Android
Provides support for integrating MenrvaEngine into Android's AudioFlinger and an App for interacting with the Engine.  Detailed Build Instructions can be found [here](../hosts/android/docs/BuildInstructions.md).
