# Contributing to Menrva

## Development Requirements
  - Android Studio
  - Android SDK
  - Android NDK
  - Java SDK (JDK)

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
    * ./adb.exe root
    * ./adb.exe remount
    * ./adb.exe shell chmod 777 /system
    * ./adb.exe shell chmod 777 /system/*
    * ./adb.exe shell chmod 777 /vendor
    * ./adb.exe shell chmod 777 /vendor/*
  - You can now push files to the system & vendor folders and subfolders, as well as modify any files in the those folders
    * ./adb.exe push %MenrvaProjectDir%/menrva/app/build/intermediates/cmake/debug/obj/x86/libMenrvaEngine.so /vendor/lib/soundfx/

### Android Platform Tools Command Line
This will simply open a command line to your Android Platform Tools folder which contains useful tools for debugging such as adb.

#### Add Android Platform Tools Command Line as External Tool
  - Open File -> Settings -> Tools -> External Tools
  - Add a new External Tools
  - Use the following settings to configure the External Tool Parameters
    * Name : Platform Tools Command Line
    * Program : cmd.exe -OR- powershell.exe
    * Working Directory : $ModuleSdkPath$\platform-tools

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

## Debugging Requirements
  - Android Studio
  - Android Virtual Device (see [Android Emulator with Writable System](#android-emulator-with-writable-system) section above)
  - AOSP Source Code & Build Environment

## Debugging Tips & Tricks
  
### Building AOSP from Source
  - See https://source.android.com/setup for General Info about AOSP Source Code, Releases & Build Process
  - Follow instructions starting from https://source.android.com/setup/build/requirements
  - If using Ubuntu 16 or higher then run the following command before the make command : ```export LC_ALL=C```
  - Quick summary of commands for setting up and building AOSP on Ubuntu 16 or higher : ```<COMING SOON>```

### Building Individual AOSP Modules from Source
  - Setup your Dev Environment & Working Directory (see [Building AOSP from Source](#building-aosp-from-source) above)
  - Using Module Name
    * Identify the Module Name from the associated Android.mk or Android.bp file
    * Run the command : make [ModuleName]
  - Using Module Path
    * Identify the Module Path
    * Run the command : mmm [ModulePath]

### Using AOSP Build for Debugging
  - Modify AOSP Source Code as necessary for Debugging (see [Relevant AOSP Source Files](#relevant-aosp-source-files) below)
  - Successfully complete an AOSP Build (see [Building AOSP from Source](#building-aosp-from-source) above)
  - Run the following commands to initialize the environment : ```. build/envsetup.sh ; lunch <Target Used in Build>```
  - Run the following command to execute the AOSP Build in the Android Emulator : ```emulator -use-system-libs -writable-system```
  - Handle any necessary setup on the Emulated Device
  - Open the Android Project in Android Studio
  - Run the Project with or without Debugging as needed

### Relevant AOSP Source Files
  - AudioEffect.java # /frameworks/base/media/java/android/media/audiofx/AudioEffect.java : https://android.googlesource.com/platform/frameworks/base/+/master/media/java/android/media/audiofx/AudioEffect.java
  - android_media_AudioEffect.cpp # /frameworks/base/media/jni/audioeffect/android_media_AudioEffect.cpp : https://android.googlesource.com/platform/frameworks/base/+/master/media/jni/audioeffect/android_media_AudioEffect.cpp
  - MORE COMING SOON

## Troubleshooting AOSP Build

### Google Repo Tool misses necessary files
  - Use the compiler error message to identify which files are missing
  - Find the remote repository with the missing files in Google's listings : https://android-review.googlesource.com/admin/repos
  - Investigate to find the root repository folder in your Working Directory
  - Delete the root repository folder
  - Clone the necessary repository using : git clone [repo-url] --branch [branch-name] --single-branch
