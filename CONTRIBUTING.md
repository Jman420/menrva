# Contributing to Menrva

## Development Requirements
  - Android Studio
  - Android SDK
  - Android NDK
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
  
## Generating Protobuf Message, Command & Handler Source Files
  - Follow above instructions for Compiling Protobuf for Android
  - Execute the [scripts/generate_protobuf_commands.ps1](scripts/generate_protobuf_commands.ps1) script to generate the Protobuf Message Source Files
  **Note:** These files are not included under source control because the generated source code versions on both sides (Java App and C++ Engine) must be the same.
  - Execute the [scripts/generate_command_handler_base.ps1](scripts/generate_command_handler_base.ps1) script to generate the TypedCommandHandlerBase Class
  **Note:** These files are not included under source control because they need to incorporate all Command Handlers that have been defined.

## Creating New App to Engine Command
  - Follow above instructions for Compiling Protobuf for Android
  - Execute the [scripts/create_new_protobuf_command.ps1](scripts/create_new_protobuf_command.ps1) script with the Command Name as an argument to generate the Protobuf Command File
  - Add necessary data points to the Request and Response Messages in the Resulting File in the [menrva/app/src/main/protobuf](menrva/app/src/main/protobuf) directory
  - Execute the [scripts/generate_protobuf_commands.ps1](scripts/generate_protobuf_commands.ps1) script to generate the Protobuf Message Source Files
  - Execute the [scripts/create_new_command_handler.ps1](scripts/create_new_command_handler.ps1) script with the Command Name as an argument to generate the C++ Command Handler
  - Add necessary logic to the Command Handler in the Resulting File in the [menrva/app/src/main/cpp/command_handlers](menrva/app/src/main/cpp/command_handlers) directory

## Configuring Calculation Precision
See [Calculation Precision](README.md#calculation-precision) for details about Calculation Precision.

### Enabling Double Precision
  - Compile FFTW with Double Precision (see [FFTW Precision](https://github.com/Jman420/fftw_for_android/blob/master/README.md#fftw-precision) for details)
  - Compile KissFFT with Double Precision (see [KissFFT Precision](https://github.com/Jman420/kissfft_for_android/blob/master/README.md#kissfft-precision) for details)
  - Uncomment the ```//#define MENRVA_DOUBLE_PRECISION``` line in [config.h](menrva/app/src/main/cpp/config.h)
  - Recompile Menrva
  - Follow the Installation Instructions of your choice
  
## Automated ADB Installation
  - Complete a Successful Build
  - Run an Android Emulated Device via AVD -OR- Attach an Android Device via USB with ADB Debugging Enabled
  - Execute [scripts/deploy_to_device.ps1](scripts/deploy_to_device.ps1) to install the build via ADB (multiple executions may be necessary for ADB to initialize)

## Generating Magisk Module Zip
  - Download latest MMT-Ex (Magisk Module Template Extended) from [https://github.com/Zackptg5/MMT-Extended](https://github.com/Zackptg5/MMT-Extended) (downloading the entire repository as a Zip File will suffice)
  - Extract the contents of the folder in the Zip File (something like 'MMT-Extended-master') to /MMT-Ex/
  - Execute the [scripts/generate_magisk_module.ps1](scripts/generate_magisk_module.ps1) script to generate the Magisk Module Zip
  - Magisk Module Zip will be in /artifacts/ directory

## AVD Tips & Tricks

### Capture Logcat from Boot
Most issues with AudioFlinger based Audio Mods will occur during device startup (between boot and the initial display of the lock screen).  The most reliable way to do this is to capture the Logcat using ADB.

  - Unlock Developer Mode on Android Device
  - Enable USB Debugging on Android Device
  - Configure Android Device to always allow USB Debugging from Target Computer
  - Shutdown Android Device
  - Open Console on Target Computer
  - Execute :
    * Powershell : ./adb.exe -d logcat | Tee-Object -File logcat_dump.txt

### Android Emulator with Writable System
Android Emulator is already deeply integrated into Android Studio through the AVD Manager and the App Launcher/Debugger.  But those integrations do not provide the ability to configure the command line parameters passed to the Emulator when it is run.  In order to make the system & vendor folders writable and persist changes between reboots we need to add the '-writable-system' command line argument.

#### Add Android Emulator as External Tool
  - Open File -> Settings -> Tools -> External Tools
  - Add a new External Tools
  - Use the following settings to configure the External Tool Parameters
    * Name : Run Project Emulator
    * Program : $ModuleSdkPath$\emulator\emulator.exe
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
