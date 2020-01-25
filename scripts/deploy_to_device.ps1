param([string]$Architecture = "x86",
      [string]$BuildType = "debug")

. ./build_variables.ps1
$MenrvaBuildApk = "$RootAppDir/build/outputs/apk/$BuildType/app-$BuildType.apk"
$AppArtifact = "$ArtifactsRootDir/MenrvaApp-$BuildType.apk"
$MenrvaLibPath = "$BackendArtifactDir/$Architecture/$MenrvaLibFileName"
$Fftw3LibPath = "$BackendArtifactDir/$Architecture/$Fftw3LibFileName"
$KissFftLibPath = "$BackendArtifactDir/$Architecture/$KissFftLibFileName"
$SharedCppLibPath = "$BackendArtifactDir/$Architecture/$SharedCppLibFileName"

function ExecuteAdbCommand([string]$logMsg, [string]$failMsg, [string]$command) {
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

ExecuteAdbCommand -logMsg "Installing Menrva App APK..." -failMsg "Failed to Install Menrva App APK!" -command "install -r -t $AppArtifact"
ExecuteAdbCommand -logMsg "Obtaining Root on Device..." -failMsg "Failed to Obtain Root on Device!" -command "root"
ExecuteAdbCommand -logMsg "Remounting Device Volumes..." -failMsg "Failed to Remount Device Volumes!" -command "remount"
ExecuteAdbCommand -logMsg "Pushing Menrva Lib to Device..." -failMsg "Failed to Push Menrva Lib to Device!" -command "push $MenrvaLibPath $SoundFxLibDir"
ExecuteAdbCommand -logMsg "Pushing FFTW Lib to Device..." -failMsg "Failed to Push FFTW Lib to Device!" -command "push $Fftw3LibPath $VendorLibDir"
ExecuteAdbCommand -logMsg "Pushing KissFFT Lib to Device..." -failMsg "Failed to Push KissFFT Lib to Device!" -command "push $KissFftLibPath $VendorLibDir"
ExecuteAdbCommand -logMsg "Pushing Shared C++ Lib to Device..." -failMsg "Failed to Push Shared C++ Lib to Device!" -command "push $SharedCppLibPath $VendorLibDir"
ExecuteAdbCommand -logMsg "Copying Std C++ Lib from System to Vendor..." -failMsg "Failed to Copy Std C++ Lib from System to Vendor!" -command "shell cp /system/lib/libstdc++.so /vendor/lib"
ExecuteAdbCommand -logMsg "Pushing Audio Effects Config to Device..." -failMsg "Failed to Push Audio Effects Config to Device!" -command "push $AudioEffectsConfigFile $VendorEtcDir"

ExecuteAdbCommand -logMsg "Setting Menrva Lib SEContext..." -failMsg "Failed to set Menrva Lib SEContext!" -command "shell chcon -v u:object_r:vendor_file:s0 $SoundFxLibDir/$MenrvaLibFileName"
ExecuteAdbCommand -logMsg "Setting FFTW Lib SEContext..." -failMsg "Failed to set FFTW Lib SEContext!" -command "shell chcon -v u:object_r:vendor_file:s0 $VendorLibDir/$Fftw3LibFileName"
ExecuteAdbCommand -logMsg "Setting KissFFT Lib SEContext..." -failMsg "Failed to set KissFFT Lib SEContext!" -command "shell chcon -v u:object_r:vendor_file:s0 $VendorLibDir/$KissFftLibFileName"
ExecuteAdbCommand -logMsg "Setting Shared C++ Lib SEContext..." -failMsg "Failed to set Shared C++ Lib SEContext!" -command "shell chcon -v u:object_r:vendor_file:s0 $VendorLibDir/$SharedCppLibFileName"
ExecuteAdbCommand -logMsg "Setting Std C++ Lib SEContext..." -failMsg "Failed to set Std C++ Lib SEContext!" -command "shell chcon -v u:object_r:vendor_file:s0 $VendorLibDir/libstdc++.so"
ExecuteAdbCommand -logMsg "Setting Audio Effects Config File SEContext..." -failMsg "Failed to set Audio Effects Config File SEContext!" -command "shell chcon -v u:object_r:vendor_configs_file:s0 $VendorEtcDir/$AudioEffectsConfigFileName"

ExecuteAdbCommand "Rebooting Device to Load Libraries..." -failMsg "Failed to Reboot Device!" -command "reboot"
Write-Output "Please wait for Device to reboot!"

$Timestamp = (Get-Date).ToString()
Write-Output "Menrva Deployed to Device at : $Timestamp"
