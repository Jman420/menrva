# menrva
Over-Engineered Tunable Android Audio Effects

## Description
This project is an Android Audio Effect Module which contains various tunable audio effects.  The goal of this project is to allow end users to easily tune the audio produced by their Android Device using powerful Digital Signal Processing algorithms.  This project is in the same realm as Viper4Android and JamesDSP.

## Effect Lists

### Implemented Effects
  - N/A

### Effects In Progress
  - Bass Boost

### Planned Effects
  - Stereo Widening
  - 15+ Band Equalizer

## Requesting an Effect
To request an effect open a GitHub Issue describing the effect which you would like.  Providing an open source implementation, algorithm, formula and/or any other documentation which may help in creating the effect is greatly appreciated.  Please note that I am not a mathmatician or electrical engineer and my background is mainly software engineering and development, so providing explanations or documentation which a computer scientist or engineer can understand will help speed up the process.

## Calculation Precision
I've chosen to default Menrva to single precision calculations meaning that all inputs and outputs are in C++ float32 type.  This decision was made mainly because Android's AudioFlinger Service expects float32 as the largest data type that it will handle.  The relevant structure in the AOSP Source Code is 'audio_buffer_t' which usually lives in the 'system/audio_effect.h' file.

To build a version of Menrva which uses C++ double type for double precision calculations see the (Configuring Calculation Precision)[CONTRIBUTING.md#configuring-calculation-precision] section of the [CONTRIBUTING](CONTRIBUTING.md) document.
  
## Contributing
See [CONTRIBUTING](CONTRIBUTING.md) document

## Special Thanks
Huge thanks goes out to all of the XDA Audio Mod developers out there and their projects for inspiring me to start my own.  An even bigger thanks goes out to those developers that Open Source their DSP algorithms so the rest of us can use them.  Unlikely that this is a complete list : 
  - [JamesDSP](https://github.com/james34602/JamesDSPManager) Dev Team ([james3460297](https://github.com/james34602)) - For providinng JamesDSP as an open source foundation
  - [FFTW3-Android](https://github.com/Lauszus/fftw3-android) Dev Team ([Lauszus](https://github.com/Lauszus)) - For inspiration and guidance on FFTW compilation for Android
  - [FFTConvolver](https://github.com/HiFi-LoFi/FFTConvolver) Dev Team ([HiFi-LoFi](https://github.com/HiFi-LoFi)) - For providing a reasonable and understandable FFT Convolver Implementation
  - [Binaural FFTConvolver](https://github.com/Bendrien/FFTConvolver) Dev Team ([Bendrien](https://github.com/Bendrien)) - For providing a reasonable and understandable Stereo FFT Convolver Implementation
  - [FFTW](http://fftw.org/) Dev Team ([FFTW](https://github.com/FFTW)) - For providing an awesome Fast Fourier Transform Library
  - [BetterExplained](https://betterexplained.com/) Website - For providing this awesome explanation of the [Fourier Transform](https://betterexplained.com/articles/an-interactive-guide-to-the-fourier-transform/)
