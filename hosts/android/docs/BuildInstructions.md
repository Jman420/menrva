# Android Host Implementation Build Instructions

## Development Requirements
  - Android Studio
  - Android SDK
  - Android NDK
  - Powershell
  
## Build Dependencies
See [CONTRIBUTING](../../../docs/CONTRIBUTING.md#building-dependencies) for details on building Project Dependencies.

## Generating Templated Android Source Code
To support MenvaEngine's Protobuf Command API the Android Implementation uses Templated Source Code to ensure compatible command structures.  Scripts have also been provided to automate using and populating the Template Source Code files when necessary.  Source Code Files which are generated from Templates will carry a header comment identifying it as generated source code and provide the path to the Source Code Template file.  These files are not tracked by source control and are explicitly ignored in the root gitignore.

### Steps
  - Use [/hosts/android/scripts/generate_protobuf_commands.ps1](../scripts/generate_protobuf_commands.ps1) to generate the necessary Java Source Code from the Protobuf Command Files

## Building the Android Implementation
Once the above steps in [Generating Templated Android Source Code](#generating-templated-android-source-code) have been completed the Android Implementation is ready to be built.  The Android Implementation provides an AndroidStudio Project for IDE supported builds as well as a Powershell script for automated builds.  Only the Powershell automated build will be covered here.

*NOTE:* The Android Implementation does not require MenrvaEngine to be pre-compiled.  Only its Generated Source Code must be provided.

### Steps
  - Generate MenrvaEngine Templated Source Code as described in [Generating Templated Engine Source Code](../../../engine/docs/DevInstructions.md#generating-templated-engine-source-code)
  - Use [/engine/scripts/copy_include_headers.ps1](/engine/scripts/copy_include_headers.ps1) to gopy the MenrvaEngine Include Headers to its Output Directory
  - Generate Android Templated Source Code as described in [Generating Templated Android Source Code](#generating-templated-android-source-code)
  - Use [/hosts/android/scripts/build.ps1](../scripts/build.ps1) to execute a Gradle build

## Generating Magisk Module
  - Perform a successful build as described in [Building the Android Implementation](#building-the-android-implementation)
  - Use [/hosts/android/scripts/prepare_artifacts.ps1](../scripts/prepare_artifacts.ps1) to prepare the build artifacts
  - Use [/hosts/android/scripts/prepare_magisk_module_template.ps1](../scripts/prepare_magisk_module_template.ps1) to download and extract the MMT-EX Template
  - Use [/hosts/android/scripts/generate_magisk_module.ps1](../scripts/generate_magisk_module.ps1) to generate the Magisk Module Zip
  - Magisk Module Zip will be in /hosts/android/artifacts/ directory

## Scripted Installation
  - Perform a successful build as described in [Building the Android Implementation](#building-the-android-implementation)
  - Use [/hosts/android/scripts/prepare_artifacts.ps1](../scripts/prepare_artifacts.ps1) to prepare the build artifacts
  - Run an Android Emulated Device via AVD -OR- Attach an Android Device via USB with ADB Debugging Enabled
  - Use [/hosts/android/scripts/deploy_to_device.ps1](../scripts/deploy_to_device.ps1) to install the build via ADB (multiple executions may be necessary for ADB to initialize)
