@ECHO OFF

SET BuildDir=build
SET OutputDir=out
SET RootSourcePath=./src

SET HostBuildDir=%BuildDir%\host
SET HostOutputDir=%OutputDir%\host

SET BuildLibPath=%HostBuildDir%\lib
SET IncludesPath=%RootSourcePath%\include

ECHO Removing Existing Build ^& Output Directories...
IF EXIST "%HostBuildDir%" (
    ECHO Removing existing Host Build Directory...
    RMDIR /S /Q "%HostBuildDir%"
)
IF EXIST "%HostOutputDir%" (
    ECHO Removing existing Host Output Directory...
    RMDIR /S /Q "%HostOutputDir%"
)

ECHO Creating Build ^& Output Directories for Host...
MKDIR %HostBuildDir%
MKDIR %HostOutputDir%

ECHO Building wxWidgets for Host Architecture...
PUSHD %HostBuildDir%
cmake ^
    -DwxBUILD_SHARED=OFF ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_INSTALL_PREFIX=..\..\%HostOutputDir% ^
    -G "Ninja" ^
    ..\..\src\

cmake --build . --target install
POPD
ECHO Successfully built Protobuf for Host Architecture!
