# Building FFTW3 for Android

The 'build_fftw3.ps1' script is a PowerShell script which will automatically generate the necessary Android Make files for ndk-build to compile FFTW3 for Android.

## Steps
  - Download the latest build of FFTW3 from www.fftw.org (do not download from the GitHub repo; those releases are missing necessary files)
  - Unzip the archive to the /fftw3/jni folder
  - Update version numbers in 'fftw3_config.h' file
    * #define PACKAGE_STRING "fftw 3.3.8"
    * #define PACKAGE_VERSION "3.3.8"
    * #define VERSION "3.3.8"
  - Execute the 'build_fftw3.ps1' script
  - Resulting files are in /fftw3/lib/ directory
