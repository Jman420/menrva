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

# Actual Data Structure & Method Dependencies
  - hardware/audio_effect.h
    * effect_interface_s
    * effect_handle_t
    * audio_effect_library_t
    * AUDIO_EFFECT_LIBRARY_INFO_SYM
    * AUDIO_EFFECT_LIBRARY_TAG
    * EFFECT_LIBRARY_API_VERSION
    * EFFECT_CONTROL_API_VERSION

  - system/audio_effect.h
    * effect_config_t
    * effect_uuid_t
    * effect_descriptor_t
    * audio_buffer_t
    * effect_param_t
    * effect_command_e (entire enum)

  - system/audio-base.h
    * audio_format_t (entire enum)

  - system/audio.h
    * audio_uuid_t

  - system/audio_effect-base.h
    * EFFECT_FLAG_TYPE_INSERT (individual enum)
    * EFFECT_FLAG_INSERT_FIRST (individual enum)
    * effect_buffer_access_e (entire enum)

  - liblog/android/log.h (entire file)
