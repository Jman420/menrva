@ECHO OFF

SET BuildDir=build
SET OutputDir=out
SET RootSourcePath=./src

SET HostBuildDir=%BuildDir%\host
SET HostOutputDir=%OutputDir%\host

SET BuildLibPath=%HostBuildDir%\lib

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
    -G "Ninja" ^
    ..\..\src\

ninja
POPD
ECHO Successfully built Protobuf for Host Architecture!

ECHO Copying Host Libraries to Output Directories...
xcopy /I /E "%BuildLibPath%" "%HostOutputDir%"
ECHO Succesfully Copied Host Libraries to Output Directories!
