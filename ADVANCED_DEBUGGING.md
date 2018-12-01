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

### TL;DR - Building AOSP from Source
  - Commands for setting up Build Environment on Ubuntu 16 or higher :  
  ```
  sudo apt install openjdk-8-jdk git-core gnupg flex bison gperf build-essential zip curl zlib1g-dev gcc-multilib g++-multilib libc6-dev-i386 lib32ncurses5-dev x11proto-core-dev libx11-dev lib32z-dev libgl1-mesa-dev libxml2-utils xsltproc unzip python-networkx

  curl https://storage.googleapis.com/git-repo-downloads/repo > ~/bin/repo
  chmod a+x ~/bin/repo

  git config --global user.name "<Your Username>"
  git config --global user.email "<Your Email>"
  ```
  - Commands for Cloning AOSP Source Code :  
  ```
  repo init -u https://android.googlesource.com/platform/manifest
  repo sync
  ```
  - Download & Extract appropriate Proprietary Binaries from https://developers.google.com/android/blobs-preview
  - Execute the Proprietary Binaries Shell Scripts from the AOSP Source Code Root Directory :  
  **_EXAMPLE ONLY_**  
  ```
  ../prop-bins/pixel2-xl/extract-google_devices-taimen.sh
  ../prop-bins/pixel2-xl/extract-qcom-taimen.sh
  ```
  - Commands for Building AOSP from Source Code executed from AOSP Source Code Root Directory :  
  ```
  make clobber
  
  . build/envsetup.sh
  lunch <Desired Target Architecture; recommend aosp_x86_64-eng>

  export LC_ALL=C
  make -j8
  ```

### Building Individual AOSP Modules from Source
  - Setup your Dev Environment & Working Directory (see [Building AOSP from Source](#building-aosp-from-source) above)
  - Using Module Name
    * Identify the Module Name from the associated Android.mk or Android.bp file
    * Run the command : ```make [ModuleName]```
  - Using Module Path
    * Identify the Module Path
    * Run the command : ```mmm [ModulePath]```

### Rebuilding System Image
  - Successfully Build AOSP from Source at least once (see [Building AOSP from Source](#building-aosp-from-source) above)
  - Make any necessary changes to Source Files
  - Rebuild any necessary AOSP Modules (see [Building Individual AOSP Modules from Source](#building-individual-aosp-modules-from-source) above)
  - Rebuild the System Image using the command : ```make systemimage```

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
**_Note_** : This is not a complete list of AOSP Source Files involved in executing the Audio Effect Pipeline, but these should provide a sufficient starting point to track down any other source files which may be involved.  Feel free to suggest adding additional items to this list.

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
