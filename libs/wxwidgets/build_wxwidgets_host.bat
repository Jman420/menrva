@ECHO OFF

SET BuildType=%1
IF "%BuildType%"=="" (
    SET BuildType=Debug
    ECHO Build Type not specified.  Using Debug configuration.
)

SET BuildDir=build
SET OutputDir=out
SET RootSourcePath=./src

SET HostBuildDir=%BuildDir%\host\%BuildType%
SET HostOutputDir=%OutputDir%\host\%BuildType%

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
    -DCMAKE_BUILD_TYPE=%BuildType% ^
    -DCMAKE_INSTALL_PREFIX=..\..\..\%HostOutputDir% ^
    -G "Ninja" ^
    ..\..\..\src\

cmake --build . --target install
POPD
ECHO Successfully built Protobuf for Host Architecture!
