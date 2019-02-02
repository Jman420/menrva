param([string]$DependencyType = "SHARED",
      [string]$Architecture = "x86",
      [string]$BuildType = "debug")

$AdbExe = "$env:LOCALAPPDATA/Android/Sdk/platform-tools/adb.exe"
$MenrvaLib = "./menrva/app/build/intermediates/cmake/$BuildType/obj/$Architecture/libMenrvaEngine.so"
$Fftw3Lib = "./fftw/out/$Architecture/libfftw3.so"
$KissFftLib = "./kissfft/out/$Architecture/libkissfft.so"
$AudioEffectsConfig = "./audio_effects.conf"
$VendorLib = "/vendor/lib/"
$VendorEtc = "/vendor/etc/"
$SoundFxLib = "/vendor/lib/soundfx/"

function ExecuteAdbCommand([string]$logMsg, [string]$failMsg, [string]$command) {
    Write-Output $logMsg
    Invoke-Expression "$AdbExe $command $parameters"
    
    if ($LASTEXITCODE -ne 0) {
        Write-Output $failMsg
        exit 1
    }
}

ExecuteAdbCommand -logMsg "Obtaining Root on Device..." -failMsg "Failed to Obtain Root on Device!" -command "root"
ExecuteAdbCommand -logMsg "Remounting Device Volumes..." -failMsg "Failed to Remount Device Volumes!" -command "remount"
ExecuteAdbCommand -logMsg "Pushing Menrva Lib to Device..." -failMsg "Failed to Push Menrva Lib to Device!" -command "push $MenrvaLib $SoundFxLib"
ExecuteAdbCommand -logMsg "Pushing FFTW Lib to Device..." -failMsg "Failed to Push FFTW Lib to Device!" -command "push $Fftw3Lib $VendorLib"
ExecuteAdbCommand -logMsg "Pushing KissFFT Lib to Device..." -failMsg "Failed to Push KissFFT Lib to Device!" -command "push $KissFftLib $VendorLib"
ExecuteAdbCommand -logMsg "Pushing Audio Effects Config to Device..." -failMsg "Failed to Push Audio Effects Config to Device!" -command "push $AudioEffectsConfig $VendorEtc"
ExecuteAdbCommand "Rebooting Device to Load Libraries..." -failMsg "Failed to Reboot Device!" -command "reboot"

Write-Output "Please wait for Device to reboot!"

$Timestamp = (Get-Date).ToString()
Write-Output "Backend Deployed to Device at : $Timestamp"
