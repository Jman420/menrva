. ./variables.ps1

$OutputDir = "../out"

$IncludeFilePattern = @($CppHeaderFileExtension)
$IncludeOutputDir = "./$OutputDir/include/menrvaEngine"
$ExcludedFolders = "test"
$ExcludedFiles = "*test*"

$EngineConfigFileName = "EngineConfig.h"

# Find Source Directories
Write-Output "Finding Source Directories to copy Include Files..."
Push-Location $RootSourceDir
$sourceDirectories = (Get-ChildItem -Path . -Directory -Recurse | Where { $_.FullName -NotMatch $ExcludedFolders }).FullName | Resolve-Path -Relative
if (!$sourceDirectories) {
    Write-Output "Error : No Source Directories found!"
    Pop-Location
    exit 1
}
Write-Output "Source Directories Found : "
Write-Output "$sourceDirectories`n"
Pop-Location

# Remove Include output directory
if (Test-Path $IncludeOutputDir) {
    Write-Output "Removing existing Output Include directory..."
    Remove-Item $IncludeOutputDir -Force -Recurse
}

# Make the Include output directory
Write-Output "Creating output Include directory..."
New-Item -ItemType directory -Force -Path $IncludeOutputDir
$includeFileDest = Resolve-Path $IncludeOutputDir

# Copy Headers to Include Directory
Write-Output "Copying Include Files to $includeFileDest ..."
foreach ($sourceDir in $sourceDirectories) {
    Push-Location $RootSourceDir/$sourceDir
    $includeFiles = (Get-ChildItem -Path $IncludeFilePattern -Exclude $ExcludedFiles).FullName
    
    foreach ($includeFile in $includeFiles) {
        $fileName = Resolve-Path -Relative -Path "$includeFile"
        $fileDestination = "$includeFileDest/$sourceDir/$fileName"
        New-Item -Force $fileDestination
        Copy-Item -Force $includeFile -Destination $fileDestination
    }
    Pop-Location
}

# Copy Engine Config
Write-Output "Copying Engine Config Header to Include Directory..."
New-Item -Force $includeFileDest/$EngineConfigFileName
Copy -Force $RootSourceDir/$EngineConfigFileName -Destination $includeFileDest/$EngineConfigFileName

Write-Output "Successfully copied Menrva Engine Include Files to $includeFileDest !"
