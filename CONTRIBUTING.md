# Contributing to Menrva

## Development Requirements
  - Android Studio
  - Android SDK
  - Android NDK
  - Java SDK (JDK)
  - FFTW3
  
## Versioning
Menrva uses [Semantic Versioning](https://semver.org/) for easy consumption in other projects if needed.  Please adhere to this versioning pattern when making contributions.  See AOSP's [Version your app](https://developer.android.com/studio/publish/versioning) guide for details on updating the app versioning.

## Compiling FFTW3 for Android
  - See instructions in [FFTW Build Readme](fftw3/README.md)
  - Update Menrva CMakeList.txt if necessary; Menrva expects an FFTW3 Shared Library by default

## Development Tools & Tips

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

### javah
A tool provided with the JDK for generating header files.  We will be using it to generate C++ JNI Header files.

#### Integrate javah into Android Studio
  - Open File -> Settings -> Tools -> External Tools
  - Add a new External Tools
  - Use the following settings to configure the External Tool Parameters
    * Name : Generate JNI Headers
    * Program : $JDKPath$\bin\javah.exe
    * Parameters : -classpath "$Classpath$" -v -jni "$FileClass$"
    * Working Directory : $ProjectFileDir$\app\src\main\cpp\jni

#### Using javah to Generate a JNI Header File
  - Create a Java Class which declares a method using the 'native' keyword
  - Right-click on the Java Class -> External Tools -> Generate JNI Headers
  - Use the newly created C++ Header File in app\main\cpp\jni to expose the necessary native methods

## Configuring Calculation Precision
See (Calculation Precision)[README.md#calculation-precision] for details about Calculation Precision.

### Enabling Double Precision
  - Compile FFTW3 with Double Precision (see [FFTW Precision](fftw3/README.md#fftw-precision) for details)
  - Uncomment the ```//#define MENRVA_DOUBLE_PRECISION``` in [config.h](menrva/app/src/main/cpp/config.h)
  - Compile Menrva
  - Resulting APK is in /menrva/app/build/outputs/apk/<debug|release>/
  - Resulting Menrva Library is in /menrva/app/build/intermediates/cmake/<debug|release>/obj/
  - Resulting FFTW3 Library is in /fftw3/out/
  - Follow Manual Installation steps
