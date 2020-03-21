# Menrva Android Implementation
The Menrva Android Implementation consists of a few components.  The Android Interface provides the Public Interface, Structures and Logic to integrate MenrvaEngine into the Android AudioFlinger System.  The Frontend App provides a UI for tuning and configuring the underlying MenrvaEngine instances.  The Android Service to support applying MenrvaEngine to all audio sessions.

## Contributing
See [CONTRIBUTING](../../docs/CONTRIBUTING.md) document for details on building and contributing to the Menrva Projects.

See [BuildInstructions](docs/BuildInstructions.md) document for details on building the Android Host Implementation Project.

See [AndroidDebugging](docs/AndroidDebugging.md) document for details on debugging the Android Host Implementation Builds.

## Installation Methods

### Scripted Installation
To ease both development and deployment of Menrva to target devices I have provided the [scripts/deploy_to_device.ps1](scripts/deploy_to_device.ps1) script which will attempt to use ADB to install Menrva onto a target device.  The script will installl both the Engine Backend and UI Frontend to the device.  The script requires a successful build of Menrva and the target device must provide root access to ADB via the 'adb root' command.  This method can also be used to deploy Menrva to an emulated device.

#### Steps
  - Successfully build Menrva (see [CONTRIBUTING](../../docs/CONTRIBUTING.md))
  - Execute [prepare_artifacts.ps1](scripts/prepare_artifacts.ps1) script
  - Execute the [deploy_to_device.ps1](scripts/deploy_to_device.ps1) script
  - Wait for the device to restart

### Magisk Installation
  - Successfully build Menrva (see [CONTRIBUTING](../../docs/CONTRIBUTING.md))
  - Execute [prepare_artifacts.ps1](scripts/prepare_artifacts.ps1) script
  - Execute the [prepare_magisk_module_template.ps1](scripts/prepare_magisk_module_template.ps1) script
  - Execute the [generate_magisk_module.ps1](scripts/generate_magisk_module.ps1) script
  - Magisk Module Zip will be at /hosts/android/artifacts/Menrva-MagiskModule.zip
  - Copy the Magisk Module Zip to your device and install with Magisk

## 64Bit Libraries
64Bit Backend Libraries are not supported for Android for two reason.  First, AudioFlinger seems to be built in 32Bit for most devices and will not load 64Bit Audio Effect Libraries.  Second, x86_64 has build issues with KFR Lib when building for Android.

The above being said, 64Bit Libraries should be fully functional if successful builds can be accomplished.

## Calculation Precision
The Menrva Android Implementation uses single precision calculations meaning that all inputs and outputs are in C++ float32 type.  This decision was made mainly because Android's AudioFlinger Service expects float32 as the largest data type that it will handle.  The relevant structure in the AOSP Source Code is 'audio_buffer_t' which usually lives in the 'system/audio_effect.h' file.

See the [Configuring Calculation Precision](../../docs/CONTRIBUTING.md#configuring-calculation-precision) section of the [CONTRIBUTING](../../docs/CONTRIBUTING.md) document for details on modifying the calculation precision.
