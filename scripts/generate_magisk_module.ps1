. ./variables.ps1
$ModuleAddonDir = "$ModuleCommonDir/addon"
$ModuleRootDummyFile = "$MagiskModuleDir/dummy.txt"
$ModuleSystemDir = "$MagiskModuleDir/system"

if (!(Test-Path $ArtifactsRootDir)) {
    Write-Output "Artifacts directory is missing.  Execute prepare_artifacts.sh and try again."
    exit 1
}
if (!(Test-Path $AppArtifact)) {
    Write-Output "MenrvaApp Apk File is missing.  Execute prepare_artifacts.sh and try again."
    exit 1
}

Write-Output "Copying Menrva APK to MMT directory..."
Copy-Item -Path "$AppArtifact" -Destination "$MagiskModuleDir" -Force

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

Write-Output "Creating Magisk Zip File based on MMT directory..."
if (Test-Path $MenrvaMagiskModuleFile) {
    Remove-Item $MenrvaMagiskModuleFile -Force
}
7z a "$MenrvaMagiskModuleFile" "$MagiskModuleDir/*" -r

Write-Output "Successfully generated Menrva Magisk Zip File : $MenrvaMagiskModuleFile !"
