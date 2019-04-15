# Contributing to Menrva

## Development Requirements
  - Android Studio
  - Android SDK
  - Android NDK
  - Java SDK (JDK)
  - KFR Lib
  - FFTW
  - KissFFT
  - Protobuf
  
## Versioning
Menrva uses [Semantic Versioning](https://semver.org/) for easy consumption in other projects if needed.  Please adhere to this versioning pattern when making contributions.  See AOSP's [Version your app](https://developer.android.com/studio/publish/versioning) guide for details on updating the app versioning.

## Compiling FFTW for Android
  - See instructions in [FFTW Build Readme](https://github.com/Jman420/fftw_for_android/blob/master/README.md)
  
## Compiling KissFFT for Android
  - See instructions in [KissFFT Build Readme](https://github.com/Jman420/kissfft_for_android/blob/master/README.md)

## Compiling Protobuf for Android
  - See instruction in [Protobuf Build Readme](https://github.com/Jman420/protobuf_for_android/blob/develop/README.md)
  
## Generating Protobuf Message Source Files
  - Follow above instructions for Compiling Protobuf for Android
  - Execute the [compile_protobuf_messages.ps1](compile_protobuf_messages.ps1) script to generate the Protobuf Message Source Files
  **Note:** These files are not included under source control because the generated source code versions on both sides (Java App and C++ Engine) must be the same.

## Configuring Calculation Precision
See [Calculation Precision](README.md#calculation-precision) for details about Calculation Precision.

### Enabling Double Precision
  - Compile FFTW with Double Precision (see [FFTW Precision](https://github.com/Jman420/fftw_for_android/blob/master/README.md#fftw-precision) for details)
  - Compile KissFFT with Double Precision (see [KissFFT Precision](https://github.com/Jman420/kissfft_for_android/blob/master/README.md#kissfft-precision) for details)
  - Uncomment the ```//#define MENRVA_DOUBLE_PRECISION``` line in [config.h](menrva/app/src/main/cpp/config.h)
  - Recompile Menrva
  - Follow Installation Instructions of your choice
  
## Automated ADB Installation
  - Complete a Successful Build
  - Run an Android Emulated Device via AVD -OR- Attach an Android Device via USB with ADB Debugging Enabled
  - Execute [deploy_to_device.ps1](deploy_to_device.ps1) to install the build via ADB (multiple executions may be necessary for ADB to initialize)

## AVD Tips & Tricks

### Android Emulator with Writable System
Android Emulator is already deeply integrated into Android Studio through the AVD Manager and the App Launcher/Debugger.  But those integrations do not provide the ability to configure the command line parameters passed to the Emulator when it is run.  In order to make the system & vendor folders writable and persist changes between reboots we need to add the '-writable-system' command line argument.

#### Add Android Emulator as External Tool
  - Open File -> Settings -> Tools -> External Tools
  - Add a new External Tools
  - Use the following settings to configure the External Tool Parameters
    * Name : Run Project Emulator
    * Program : $ModuleSdkPath$\tools\emulator.exe
    * Parameters : -avd $ProjectName$ -writable-system

#### Add AVD for Project
  - Open Tools -> AVD Manager
  - Create a New Virtual Device
  - Select a Profile which does NOT support Google Play Store
  - Name AVD exactly the same as your Project's Name
  - Finish creating the AVD

#### Running the Project Emulator
  - Open Tools -> External Tools -> Run Project Emulator
  OR
  - Right Click -> External Tools -> Run Project Emulator

#### Running a Project on the Project Emulator
  - Run the Project Emulator (see [Running the Project Emulator](#running-the-project-emulator) above)
  - Run the Project via Android Studio
  - Select the Android Device under 'Connected Devices' named after your Project when prompted for a Deployment Target

#### Making changes to the Project Emulator System Folder
  - Run the Project Emulator (see [Running the Project Emulator](#running-the-project-emulator) above)
  - Open a command line to your Android Platform Tools
  - Execute the following commands :  
    ```
    ./adb.exe root
    ./adb.exe remount
    ./adb.exe shell chmod 777 /system
    ./adb.exe shell chmod 777 /system/*
    ./adb.exe shell chmod 777 /vendor
    ./adb.exe shell chmod 777 /vendor/*
    ```
  - You can now push files to the system & vendor folders and subfolders, as well as modify any files in the those folders, such as :  
  ```./adb.exe push %MenrvaProjectDir%/menrva/app/build/intermediates/cmake/debug/obj/x86/libMenrvaEngine.so /vendor/lib/soundfx/```
