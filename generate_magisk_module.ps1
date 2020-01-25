param([string]$BuildType = "debug")

. ./build_variables.ps1
$ApkFile = "$ArtifactsRoot/MenrvaApp-$BuildType.apk"

if (!Test-Path $ArtifactsDir) {
    Write-Output "Artifacts directory is missing.  Execute extract_artifacts.sh and try again."
    exit 1
}
if (!Test-Path $ApkFile) {
    Write-Output "MenrvaApp Apk File is missing.  Execute extract_artifacts.sh and try again."
    exit 1
}
if (!Test-Path $BackendArtifactDir) {
    Write-Output "Backend Artifacts Directory is missing.  Execute extract_artifacts.sh and try again."
    exit 1
}
if ((Get-ChildItem -Path $BackendArtifactDir -Directory).Count < 1) {
    Write-Output "Backend Artifacts are missing.  Execute extract_artifacts.sh and try again."
    exit 1
}

New-Item -Path "$ModuleBackendDir" -ItemType Directory -Force
Copy-Item -Path "$BackendArtifactDir/*" -Recurse -Exclude "$BackendArtifactExcludePattern" -Destination "$ModuleBackendDir" -Force

New-Item -Path "$ModuleAppDir" -ItemType Directory -Force
Copy-Item -Path "$ApkFile" -Destination "$ModuleAppDir/$ModuleApkFileName" -Force

Copy-Item -Path "$ModuleCustomizeFile" -Destination "$MagiskModuleDir" -Force
Copy-Item -Path "$ModuleInstallScriptFile" -Destination "$ModuleCommonDir" -Force
Copy-Item -Path "$ModuleInfoFile" -Destination "$MagiskModuleDir" -Force
Copy-Item -Path "$ModuleReadmeFile" -Destination "$MagiskModuleDir" -Force

Remove-Item "$ModuleCommonDir" -Force
Remove-Item "$MagiskModuleDir/*.git*" -Force
Remove-Item "$MagiskModuleDir/dummy.txt" -Force
Remove-Item "$MagiskModuleDir/customize.sh" -Force
Remove-Item "$MagiskModuleDir/system/placeholder" -Force

Compress-Archive -Path "$MagiskModuleDir/*" -DestinationPath "$ArtifactsDir/Menrva_MagiskModule.zip" -Force
