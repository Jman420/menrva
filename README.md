# menrva
Over-Engineered Tunable Android Audio Effects

## Description
This project is an Android Audio Effect Module which contains various tunable audio effects.  The goal of this project is to allow end users to easily tune the audio produced by their Android Device using powerful Digital Signal Processing algorithms.  This project is in the same realm as Viper4Android and JamesDSP.

## Effect Lists

### Implemented Effects
  - Bass Boost

### Effects In Progress
  - N/A (See [TODO List](TODO.md))

### Planned Effects
  - Master Limiter
  - Stereo Widening
  - 15+ Band Equalizer

## Requesting an Effect
To request an effect open a GitHub Issue describing the effect which you would like.  Providing an open source implementation, algorithm, formula and/or any other documentation which may help in creating the effect is greatly appreciated.  Please note that I am not a mathmatician or electrical engineer and my background is mainly software engineering and development, so providing explanations or documentation which a computer scientist or engineer can understand will help speed up the process.

## Installation Methods

### Scripted Installation
To ease both development and deployment of Menrva to target devices I have provided the [deploy_to_device.ps1](deploy_to_device.ps1) script which will attempt to use ADB to install Menrva onto a target device.  The script will installl both the Engine Backend and UI Frontend to the device.  The script requires a successful build of Menrva and the target device must provide root access to ADB via the 'adb root' command.  This method can also be used to deploy Menrva to an emulated device.

#### Scripted Installation Steps
  - Successfully build Menrva (see [CONTRIBUTING](CONTRIBUTING.md))
  - Execute the [deploy_to_device.ps1](deploy_to_device.ps1)
  - Wait for the device to restart

### In-App Installation
**Coming Soon**

### Magisk Installation
**Under Consideration**

## Calculation Precision
I've chosen to default Menrva to single precision calculations meaning that all inputs and outputs are in C++ float32 type.  This decision was made mainly because Android's AudioFlinger Service expects float32 as the largest data type that it will handle.  The relevant structure in the AOSP Source Code is 'audio_buffer_t' which usually lives in the 'system/audio_effect.h' file.

To build a version of Menrva which uses C++ double type for double precision calculations see the [Configuring Calculation Precision](CONTRIBUTING.md#configuring-calculation-precision) section of the [CONTRIBUTING](CONTRIBUTING.md) document.
  
## Contributing
See [CONTRIBUTING](CONTRIBUTING.md) document

## Special Thanks
Huge thanks goes out to all of the XDA Audio Mod developers out there and their projects for inspiring me to start my own.  An even bigger thanks goes out to those developers that Open Source their DSP algorithms so the rest of us can use them.  Hopefully this is a complete list, if not please contact me : 
  - [JamesDSP](https://github.com/james34602/JamesDSPManager) - For providing JamesDSP as an open source foundation and inspiration
  - [FFTW3-Android](https://github.com/Lauszus/fftw3-android) - For inspiration and guidance on FFTW compilation for Android
  - [BetterExplained](https://betterexplained.com/) - For providing this awesome explanation of the [Fourier Transform](https://betterexplained.com/articles/an-interactive-guide-to-the-fourier-transform/)
  - [FFTW](http://fftw.org/) - For providing an awesome Fast Fourier Transform Library
  - [KissFFT](https://github.com/mborgerding/kissfft) - For providing an awesome Fast Fourier Transform Library
  - [KFR](https://www.kfrlib.com/) - For providing an awesome Fast Fourier Transform & DSP Library
  - [FFTConvolver](https://github.com/HiFi-LoFi/FFTConvolver) - For providing a reasonable and understandable FFT Convolver Implementation
  - [Binaural FFTConvolver](https://github.com/Bendrien/FFTConvolver) - For providing a reasonable and understandable Stereo FFT Convolver Implementation
  - [Protobuf](https://github.com/protocolbuffers/protobuf) - For providing an off the shelf Protocol Serializer
  - [Dagger](https://github.com/google/dagger) - For providing an excellent Dependency Injection Framework
