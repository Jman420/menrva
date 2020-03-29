# Menrva TestHarness
The Menrva TestHarness is a cross-platform cmake project which is used to debug the Menrva Engine on a Host Machine rather than through an emulator.  This allows for faster debugging and easier resource profiling.

## Installing & Configuring wxWidgets
The Menrva TestHarness Project expects wxWidgets to be prebuilt and installed on the Host Machine.  The wxWidgets Package is located using the cmake 'find_package()' method.  See [https://www.wxwidgets.org/](https://www.wxwidgets.org/) for obtaining wxWidgets.  See [https://cmake.org/cmake/help/latest/module/FindwxWidgets.html](https://cmake.org/cmake/help/latest/module/FindwxWidgets.html) for details on configuring CMake to find wxWidgets.

## Missing wxWidgets Libraries when Executing
On Windows cmake does not automatically copy the necessary libraries to the build directory.  To workaround this try executing the 'install' target to attempt to use cmake's fixup_bundle() to copy the necessary libraries.  On Windows this may error on some Microsoft DLLs and fail verify_app(), but the result is usually a working executable.