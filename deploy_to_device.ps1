param([string]$DependencyType = "SHARED",
      [string]$Architecture = "x86",
      [string]$BuildType = "debug")

$AdbExe = "$env:LOCALAPPDATA/Android/Sdk/platform-tools/adb.exe"
$MenrvaLib = "./menrva/app/build/intermediates/cmake/$BuildType/obj/$Architecture/libMenrvaEngine.so"
$Fftw3Lib = "./fftw3/out/$Architecture/libfftw3.so"
$AudioEffectsConfig = "./audio_effects.conf"
$VendorLib = "/vendor/lib/"
$VendorEtc = "/vendor/etc/"
$SoundFxLib = "/vendor/lib/soundfx/"

Write-Output "Obtaining Root on Device..."
. $AdbExe root
. $AdbExe remount

Write-Output "Pushing Menrva Lib & Dependencies to Device..."
. $AdbExe push $MenrvaLib $SoundFxLib
if ($DependencyType.ToUpper() -eq "SHARED") {
    . $AdbExe push $Fftw3Lib $VendorLib
}

Write-Output "Pushing Audio Effects Config to Device..."
. $AdbExe push $AudioEffectsConfig $VendorEtc

Write-Output "Rebooting Device to Load Libraries..."
. $AdbExe reboot

Write-Output "Successfully Deployed Menrva to Device!"
Write-Output "Please wait for Device to reboot."
