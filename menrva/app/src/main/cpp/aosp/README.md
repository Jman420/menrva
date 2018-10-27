# About These Folders
The contents of these folders have been currated from the AOSP Repositories listed below specifically for the purpose of building Android Audio Effects which can be applied globally to all Audio Playback.  The include compiler directives have been modified to refer to the files within these folders rather than their original references.  This usually involves replacing include paths using <> characters with "" and updating the file paths.

| Folder   | Repository URL                                                  | Original Repo Path         |
|----------|-----------------------------------------------------------------|----------------------------|
| cutils   | https://android.googlesource.com/platform/system/core/          | .\libcutils\include\cutils |
| liblog   | https://android.googlesource.com/platform/system/core/          | .\liblog                   |
| hardware | https://android.googlesource.com/platform/hardware/libhardware/ | .\include\hardware         |
| system   | https://android.googlesource.com/platform/system/media/         | .\audio\include\system     |

# Examples of Android Audio Effects
 - https://android.googlesource.com/platform/frameworks/av/+/master/media/libeffects/
 - https://github.com/james34602/JamesDSPManager
 - https://github.com/omnirom/android_packages_apps_DSPManager

# UUIDs from OpenSLES_IID.h for Library Type
 - https://chromium.googlesource.com/android_tools/+/master/sdk/sources/android-25/android/media/audiofx/AudioEffect.java
