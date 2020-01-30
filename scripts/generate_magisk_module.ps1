param([string]$BuildType = "debug")

. ./build_variables.ps1
$ApkFile = "$ArtifactsRootDir/MenrvaApp-$BuildType.apk"
$ModuleAddonDir = "$ModuleCommonDir/addon"
$ModuleRootDummyFile = "$MagiskModuleDir/dummy.txt"
$ModuleSystemDir = "$MagiskModuleDir/system"

if (!(Test-Path $ArtifactsRootDir)) {
    Write-Output "Artifacts directory is missing.  Execute prepare_artifacts.sh and try again."
    exit 1
}
if (!(Test-Path $ApkFile)) {
    Write-Output "MenrvaApp Apk File is missing.  Execute prepare_artifacts.sh and try again."
    exit 1
}
if (!(Test-Path $BackendArtifactDir)) {
    Write-Output "Backend Artifacts Directory is missing.  Execute prepare_artifacts.sh and try again."
    exit 1
}
if ((Get-ChildItem -Path $BackendArtifactDir -Directory).Count -lt 1) {
    Write-Output "Backend Artifacts are missing.  Execute prepare_artifacts.sh and try again."
    exit 1
}

Write-Output "Copying Backend Libraries to MMT directory..."
New-Item -Path "$ModuleBackendDir" -ItemType Directory -Force
Copy-Item -Path "$BackendArtifactDir/*" -Recurse -Exclude "$BackendArtifactExcludePattern" -Destination "$ModuleBackendDir" -Force

Write-Output "Copying Menrva APK to MMT directory..."
Copy-Item -Path "$ApkFile" -Destination "$MagiskModuleDir/$ModuleApkFileName" -Force

Write-Output "Copying Menrva Magisk Module Files into MMT directory..."
Copy-Item -Path "$ModuleFilesDir/*" -Destination "$MagiskModuleDir" -Recurse -Force
Copy-Item -Path "$ModuleReadmeFile" -Destination "$MagiskModuleDir" -Force

Write-Output "Removing unused files from MMT directory..."
if (Test-Path $ModuleAddonDir) {
    Remove-Item "$ModuleAddonDir" -Recurse -Force
}
if (Test-Path $ModuleRootDummyFile) {
    Remove-Item "$ModuleRootDummyFile" -Force
}
if (Test-Path $ModuleSystemDir) {
    Remove-Item "$ModuleSystemDir" -Recurse -Force
}
Remove-Item "$MagiskModuleDir/*.git*" -Force

#Write-Output "Creating Magisk Zip File based on MMT directory..."
#Compress-Archive -Path "$MagiskModuleDir/*" -DestinationPath "$MenrvaMagiskModuleFile" -CompressionLevel "Fastest" -Force

Write-Output "Successfully generated Menrva Magisk Zip File : $MenrvaMagiskModuleFile !"
