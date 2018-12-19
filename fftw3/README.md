# Building FFTW3 for Android

The 'build_fftw3.ps1' script is a PowerShell script which will automatically generate the necessary Android Make files for ndk-build to compile FFTW3 for Android.

## Steps
  - Download the latest build of FFTW3 from www.fftw.org
  - Unzip the archive to the /fftw3/jni folder
  - Execute the 'build_fftw3.ps1' script
  - Resulting files are in /fftw3/obj/local/ directory

## Changing Output Library Type
  - The 'LibraryType' argument for the 'build_fftw3.ps1' script controls the type of Library that is built.  By default the script builds Static Libraries.  To build Shared Libraries simply specify the parameter '-LibraryType SHARED' when executing the build script.
