$LibraryType = "STATIC"
$FFTW3_SourcePath = "./jni"
$FFTW3_MakeFileName = "Android.mk"
$FFTW3_Source_ConfigFileName = "fftw3_config.h"
$FFTW3_Destination_ConfigFileName = "config.h"
$ExcludedFolders = "test|simd|mpi|support|threads|tools"
$NdkBuild = "Android/Sdk/ndk-bundle/ndk-build.cmd"
$BuildRoot = "./obj/local/"
$BuildOutput = "../../lib"
$IncludeFile = "$FFTW3_SourcePath/api/fftw3.h"
$IncludeFileDest = "./lib/include/fftw3.h"

# Find Source Directories
Push-Location $FFTW3_SourcePath
$sourceDirectories = (Get-ChildItem -Path . -Directory -Recurse | Where { $_.FullName -NotMatch $ExcludedFolders }).FullName | Resolve-Path -Relative
if (!$sourceDirectories) {
    Write-Output "Error : No Source Directories found!"
    Pop-Location
    exit 1
}
Write-Output "Source Directories Found : "
Write-Output "$sourceDirectories`n"

# Write Root Make File Header
Write-Output "Assembling Root Android Make File Header..."
$rootMakeContents = @"
LOCAL_PATH := `$(call my-dir)
include `$(CLEAR_VARS)


"@
Write-Output "Successfully assembled Root Android Make File Header!`n"

$C_Directories = ""
foreach ($sourceDir in $sourceDirectories) {
    Write-Output "Finding Source Files in directory : $sourceDir ..."
    $sourceFiles = (Get-ChildItem -Path $sourceDir/*.c).Name
    
    if ($sourceFiles) {
        # Create Subdirectory Make File
        Write-Output "Assembling Android Make File for directory : $sourceDir ..."
        $cleanSourceDir = $sourceDir.Replace(".\", "").Replace("\", "/")
        $dirMakeContents = "sources :="
        foreach ($sourceFile in $sourceFiles) {
            $dirMakeContents = $dirMakeContents + " $sourceFile"
        }
        $dirMakeContents = $dirMakeContents + "`n"
        $dirMakeContents = $dirMakeContents + "LOCAL_SRC_FILES += `$(addprefix $cleanSourceDir/, `$(sources))`n"
        Write-Output "Creating Android Make File for directory : $sourceDir ..."
        Out-File -FilePath $sourceDir/$FFTW3_MakeFileName -InputObject $dirMakeContents -Encoding ASCII
        Write-Output "Successfully created Android Make File for directory : $sourceDir !"
        
        # Add Subdirectory Make File to Root Make File
        Write-Output "Assembling entry to Root Android Make File for directory : $sourceDir ..."
        $rootMakeContents = $rootMakeContents + "include `$(LOCAL_PATH)/$cleanSourceDir/$FFTW3_MakeFileName`n"
        $C_Directories = $C_Directories + " `$(LOCAL_PATH)/$cleanSourceDir"
        Write-Output "Successfully assembled entry to Root Android Make File for directory : $sourceDir !"
    }
    else {
        Write-Output "No Source Files found. Skipping directory : $sourceDir !"
    }
    
    Write-Output ""
}

# Write Root Make File Footer
Write-Output "Assembling Root Android Make File Footer..."
$rootMakeContents = $rootMakeContents + "`n"
$rootMakeContents = $rootMakeContents + "LOCAL_MODULE := fftw3`n"
$rootMakeContents = $rootMakeContents + "LOCAL_C_INCLUDES :=  `$(LOCAL_PATH)/"
$rootMakeContents = $rootMakeContents + "$C_Directories`n`n"
$rootMakeContents = $rootMakeContents + "include `$(BUILD_" + $LibraryType.ToUpper() + "_LIBRARY)"
Write-Output "Creating Root Android Make File..."
Out-File -FilePath $FFTW3_MakeFileName -InputObject $rootMakeContents -Encoding ASCII
Write-Output "Successfully created Root Android Make File!`n"

# Copy config.h file into Root Source Directory
Write-Output "Copying FFTW3 Config File to Source Root..."
Copy-Item -Path ../$FFTW3_Source_ConfigFileName -Destination $FFTW3_Destination_ConfigFileName
Write-Output "Successfully copied FFTW3 Config File to Source Root!`n"
Pop-Location

# Execute ndk-build on FFTW
Write-Output "Executing ndk-build..."
. $env:LOCALAPPDATA\$NdkBuild
Write-Output "NDK-Build completed!"

# Copy Build Output to more convenient location
Write-Output "Copying Build Output to $BuildRoot ..."
Push-Location $BuildRoot
$libraryFiles = (Get-ChildItem -Path ./*.a -Recurse).FullName | Resolve-Path -Relative
foreach ($libFile in $libraryFiles) {
    $libFileDest = "$BuildOutput/" + $libFile.Replace(".\", "").Replace("\", "/")
    Write-Output "Copying $libFile to $libFileDest ..."
    New-Item -Force $libFileDest
    Copy-Item -Force $libFile -Destination $libFileDest
}
Pop-Location
Write-Output "Successfully copied Build Output to $BuildOutput !"

# Copy Include Files
Write-Output "Copying API Header to $IncludeFileDest ..."
New-Item -Force $IncludeFileDest
Copy-Item -Force $IncludeFile -Destination $IncludeFileDest
Write-Output "Successfully copied API Header to $IncludeFileDest !"
