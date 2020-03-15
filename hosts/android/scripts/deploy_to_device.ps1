param([string]$Architecture = "x86",
      [string]$BuildType = "debug",
      [string]$EffectsConfigFile = "/vendor/etc/audio_effects.xml")

. ./variables.ps1
$MenrvaBuildApk = "$AndroidAppDir/build/outputs/apk/$BuildType/app-$BuildType.apk"
$AppArtifact = "$ArtifactsRootDir/MenrvaApp-$BuildType.apk"
$MenrvaLibPath = "$BackendArtifactDir/$Architecture/$MenrvaLibFileName"
$SharedCppLibPath = "$BackendArtifactDir/$Architecture/$SharedCppLibFileName"
$EffectsConfigFileName = Split-Path -Leaf $EffectsConfigFile

$EffectLibraryName = "menrvaModule"
$EffectEngineName = "menrvaEngine"
$EffectUUID = "a91fdfe4-d09e-11e8-a8d5-f2801f1b9fd1"

function ExecuteAdbCommand([string]$logMsg, [string]$failMsg, [string]$command, [string]$parameters) {
  Write-Output $logMsg
  Invoke-Expression "$AdbExe $command $parameters"

  if ($LASTEXITCODE -ne 0) {
    Write-Output $failMsg
    exit 1
  }
}

if (!(Test-Path $ArtifactsRootDir)) {
  Write-Output "Artifacts directory is missing.  Execute prepare_artifacts.ps1 and try again."
  exit 1
}

ExecuteAdbCommand -command "install" -parameters "-r -t $AppArtifact"                      -logMsg "Installing Menrva App APK..." -failMsg "Failed to Install Menrva App APK!"
ExecuteAdbCommand -command "root"                                                          -logMsg "Obtaining Root on Device..." -failMsg "Failed to Obtain Root on Device!"
ExecuteAdbCommand -command "remount"                                                       -logMsg "Remounting Device Volumes..." -failMsg "Failed to Remount Device Volumes!"
ExecuteAdbCommand -command "push"    -parameters "$MenrvaLibPath $SoundFxLibDir"           -logMsg "Pushing Menrva Lib to Device..." -failMsg "Failed to Push Menrva Lib to Device!"
ExecuteAdbCommand -command "push"    -parameters "$SharedCppLibPath $VendorLibDir"         -logMsg "Pushing Shared C++ Lib to Device..." -failMsg "Failed to Push Shared C++ Lib to Device!"

ExecuteAdbCommand -command "shell"   -parameters "chcon -v u:object_r:vendor_file:s0 $SoundFxLibDir/$MenrvaLibFileName"   -logMsg "Setting Menrva Lib SEContext..." -failMsg "Failed to set Menrva Lib SEContext!"
ExecuteAdbCommand -command "shell"   -parameters "chcon -v u:object_r:vendor_file:s0 $VendorLibDir/$SharedCppLibFileName" -logMsg "Setting Shared C++ Lib SEContext..." -failMsg "Failed to set Shared C++ Lib SEContext!"

ExecuteAdbCommand -command "pull" -parameters "$EffectsConfigFile ." -logMsg "Pulling Effects Config File from Device..." -failMsg "Failed to pull Effects Config File from Device : $EffectsConfigFile"    
Write-Output "Patching Effects Config File on PC : ./$EffectsConfigFileName"
[System.Collections.ArrayList]$configContents = Get-Content $EffectsConfigFileName
switch -Wildcard ($EffectsConfigFile) {
  "*.xml" {
    $configContents = $configContents -NotMatch "menrva"
    $configContents = $configContents -Replace "<libraries>", "$&`n        <library name=`"$EffectLibraryName`" path=`"$MenrvaLibFileName`"/>"
    $configContents = $configContents -Replace "<effects>", "$&`n        <effect name=`"$EffectEngineName`" library=`"$EffectLibraryName`" uuid=`"$EffectUUID`"/>"
    break
  }
  "*.conf" {
    $moduleLineIndex = $configContents.IndexOf("  menrvaModule {")
    if ($moduleLineIndex -gt 0) {
      $configContents.RemoveRange($moduleLineIndex, 3)
    }
    $engineLineIndex = $configContents.IndexOf("  menrvaEngine {")
    if ($engineLineIndex -gt 0) {
      $configContents.RemoveRange($engineLineIndex, 4)
    }
    $configContents = $configContents -Replace "libraries {", "$&`n  $EffectLibraryName {\n    path /vendor/lib/soundfx/$MenrvaLibFileName\n  }"
    $configContents = $configContents -Replace "effects {", "$&`n  $EffectEngineName {\n    library $EffectLibraryName\n    uuid $EffectUUID\n  }"
    break
  }
}
Set-Content -Value $configContents -Path $EffectsConfigFileName
Write-Output "Successfully Patched Effects Config File on PC : ./$EffectsConfigFileName"

ExecuteAdbCommand -command "push" -parameters "./$EffectsConfigFileName $EffectsConfigFile" -logMsg "Pushing Patched Effect Config File to Device..." -failMsg "Failed to push Patched Effect Config File to Device!"

Write-Output "Cleaning Up Effect Config File on PC..."
Remove-Item $EffectsConfigFileName -Force

ExecuteAdbCommand "Rebooting Device to Load Libraries..." -failMsg "Failed to Reboot Device!" -command "reboot"
Write-Output "Please wait for Device to reboot!"

$Timestamp = (Get-Date).ToString()
Write-Output "Menrva Deployed to Device at : $Timestamp"
