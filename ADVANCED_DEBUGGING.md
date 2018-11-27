# Debugging AOSP System Native Code

## Debugging Requirements
  - Android Studio
  - AOSP Source Code & Build Environment

## Debugging Tips & Tricks
  
### Building AOSP from Source
  - See https://source.android.com/setup for General Info about AOSP Source Code, Releases & Build Process
  - Follow instructions starting from https://source.android.com/setup/build/requirements
  - If using Ubuntu 16 or higher then run the following command before the make command :  
  ```export LC_ALL=C```
  - Quick summary of commands for setting up and building AOSP on Ubuntu 16 or higher :  
  ```<COMING SOON>```

### Building Individual AOSP Modules from Source
  - Setup your Dev Environment & Working Directory (see [Building AOSP from Source](#building-aosp-from-source) above)
  - Using Module Name
    * Identify the Module Name from the associated Android.mk or Android.bp file
    * Run the command : ```make [ModuleName]```
  - Using Module Path
    * Identify the Module Path
    * Run the command : ```mmm [ModulePath]```

### Using AOSP Build for Debugging
  - Modify AOSP Source Code as necessary for Debugging (see [Relevant AOSP Source Files](#relevant-aosp-source-files) below)
  - Successfully complete an AOSP Build (see [Building AOSP from Source](#building-aosp-from-source) above)
  - Run the following commands to initialize the environment :  
  ```. build/envsetup.sh ; lunch <Target Used in Build>```
  - Run the following command to execute the AOSP Build in the Android Emulator :  
  ```emulator -use-system-libs -writable-system```
  - Handle any necessary setup on the Emulated Device
  - Open the Android Project in Android Studio
  - Run the Project with or without Debugging as needed

### Relevant AOSP Source Files
Note : This is not a complete list of AOSP Source Files involved in executing the Audio Effect Pipeline, but these should provide a sufficient starting point to track down any other source files which may be involved.  Feel free to suggest adding additional items to this list.

  | File Name                     | Full Source Path                                                     | Full Repo URL                                                                                                          |
  |-------------------------------|----------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------|
  | AudioEffect.java              | /frameworks/base/media/java/android/media/audiofx/AudioEffect.java   | https://android.googlesource.com/platform/frameworks/base/+/master/media/java/android/media/audiofx/AudioEffect.java   |
  | android_media_AudioEffect.cpp | /frameworks/base/media/jni/audioeffect/android_media_AudioEffect.cpp | https://android.googlesource.com/platform/frameworks/base/+/master/media/jni/audioeffect/android_media_AudioEffect.cpp |
  | AudioEffect.cpp               | /frameworks/av/media/libaudioclient/AudioEffect.cpp                  | https://android.googlesource.com/platform/frameworks/av/+/master/media/libaudioclient/AudioEffect.cpp                  |
  | AudioFlinger.cpp              | /frameworks/av/services/audioflinger/AudioFlinger.cpp                | https://android.googlesource.com/platform/frameworks/av/+/master/services/audioflinger/AudioFlinger.cpp                |
  | EffectsFactoryHalHidl.cpp     | /frameworks/av/media/libaudiohal/4.0/EffectsFactoryHalHidl.cpp       | https://android.googlesource.com/platform/frameworks/av/+/master/media/libaudiohal/4.0/EffectsFactoryHalHidl.cpp       |
  | Threads.cpp                   | /frameworks/av/services/audioflinger/Threads.cpp                     | https://android.googlesource.com/platform/frameworks/av/+/master/services/audioflinger/Threads.cpp                     |
  | Threads.h                     | /frameworks/av/services/audioflinger/Threads.h                       | https://android.googlesource.com/platform/frameworks/av/+/master/services/audioflinger/Threads.h                       |
  | Effects.cpp                   | /frameworks/av/services/audioflinger/Effects.cpp                     | https://android.googlesource.com/platform/frameworks/av/+/master/services/audioflinger/Effects.cpp                     |
