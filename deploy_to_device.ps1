param([string]$Architecture = "x86",
      [string]$BuildType = "debug")

$MenrvaBuildApk = "./menrva/app/build/outputs/apk/$BuildType/app-$BuildType.apk"
$ArtifactsRoot = "./artifacts"
$AppArtifact = "$ArtifactsRoot/MenrvaApp-$BuildType.apk"
$BackendArtifactDir = "$ArtifactsRoot/backend"
      
$AdbExe = "$env:LOCALAPPDATA/Android/Sdk/platform-tools/adb.exe"
$MenrvaLibFile = "libMenrvaEngine.so"
$MenrvaLibPath = "$BackendArtifactDir/$Architecture/$MenrvaLibFile"
$Fftw3LibFile = "libfftw3.so"
$Fftw3LibPath = "$BackendArtifactDir/$Architecture/$Fftw3LibFile"
$KissFftLibFile = "libkissfft.so"
$KissFftLibPath = "$BackendArtifactDir/$Architecture/$KissFftLibFile"
$SharedCppLibFile = "libc++_shared.so"
$SharedCppLibPath = "$BackendArtifactDir/$Architecture/$SharedCppLibFile"
$AudioEffectsConfigFile = "audio_effects.xml"
$AudioEffectsConfigPath = "./$AudioEffectsConfigFile"
$VendorLib = "/vendor/lib"
$VendorEtc = "/vendor/etc"
$SoundFxLib = "/vendor/lib/soundfx"

function ExecuteAdbCommand([string]$logMsg, [string]$failMsg, [string]$command) {
    Write-Output $logMsg
    Invoke-Expression "$AdbExe $command $parameters"
    
    if ($LASTEXITCODE -ne 0) {
        Write-Output $failMsg
        exit 1
    }
}

Write-Output "Clearing Existing Artifacts..."
if (Test-Path -Path $ArtifactsRoot) {
    Remove-Item $ArtifactsRoot -Recurse -Force
}
New-Item -ItemType directory -Path $ArtifactsRoot

Write-Output "Copying Menrva App APK to Artifacts Directory..."
Copy-Item -Path $MenrvaBuildApk -Destination $AppArtifact

Write-Output "Extracting Backend from Menrva App APK..."
Add-Type -Assembly System.IO.Compression.FileSystem
$appArtifactPath = Resolve-Path $AppArtifact
$apkZip = [IO.Compression.ZipFile]::OpenRead("$appArtifactPath")
$backendFileEntries = $apkZip.Entries | where { $_.FullName -like "lib/*" }
foreach ($backendFileEntry in $backendFileEntries) {
    $backendOutputFile = $backendFileEntry.FullName.Replace("lib/", "")
    $outputFile = "$BackendArtifactDir/$backendOutputFile"
    New-Item -Force $outputFile
    $destination = Resolve-Path $outputFile
    [IO.Compression.ZipFileExtensions]::ExtractToFile($backendFileEntry, $destination, $true) 
}
$apkZip.Dispose()

ExecuteAdbCommand -logMsg "Installing Menrva App APK..." -failMsg "Failed to Install Menrva App APK!" -command "install -r -t $AppArtifact"
ExecuteAdbCommand -logMsg "Obtaining Root on Device..." -failMsg "Failed to Obtain Root on Device!" -command "root"
ExecuteAdbCommand -logMsg "Remounting Device Volumes..." -failMsg "Failed to Remount Device Volumes!" -command "remount"
ExecuteAdbCommand -logMsg "Pushing Menrva Lib to Device..." -failMsg "Failed to Push Menrva Lib to Device!" -command "push $MenrvaLibPath $SoundFxLib"
ExecuteAdbCommand -logMsg "Pushing FFTW Lib to Device..." -failMsg "Failed to Push FFTW Lib to Device!" -command "push $Fftw3LibPath $VendorLib"
ExecuteAdbCommand -logMsg "Pushing KissFFT Lib to Device..." -failMsg "Failed to Push KissFFT Lib to Device!" -command "push $KissFftLibPath $VendorLib"
ExecuteAdbCommand -logMsg "Pushing Shared C++ Lib to Device..." -failMsg "Failed to Push Shared C++ Lib to Device!" -command "push $SharedCppLibPath $VendorLib"
ExecuteAdbCommand -logMsg "Copying Std C++ Lib from System to Vendor..." -failMsg "Failed to Copy Std C++ Lib from System to Vendor!" -command "shell cp /system/lib/libstdc++.so /vendor/lib"
ExecuteAdbCommand -logMsg "Pushing Audio Effects Config to Device..." -failMsg "Failed to Push Audio Effects Config to Device!" -command "push $AudioEffectsConfigPath $VendorEtc"

ExecuteAdbCommand -logMsg "Setting Menrva Lib SEContext..." -failMsg "Failed to set Menrva Lib SEContext!" -command "shell chcon -v u:object_r:vendor_file:s0 $SoundFxLib/$MenrvaLibFile"
ExecuteAdbCommand -logMsg "Setting FFTW Lib SEContext..." -failMsg "Failed to set FFTW Lib SEContext!" -command "shell chcon -v u:object_r:vendor_file:s0 $VendorLib/$Fftw3LibFile"
ExecuteAdbCommand -logMsg "Setting KissFFT Lib SEContext..." -failMsg "Failed to set KissFFT Lib SEContext!" -command "shell chcon -v u:object_r:vendor_file:s0 $VendorLib/$KissFftLibFile"
ExecuteAdbCommand -logMsg "Setting Shared C++ Lib SEContext..." -failMsg "Failed to set Shared C++ Lib SEContext!" -command "shell chcon -v u:object_r:vendor_file:s0 $VendorLib/$SharedCppLibFile"
ExecuteAdbCommand -logMsg "Setting Std C++ Lib SEContext..." -failMsg "Failed to set Std C++ Lib SEContext!" -command "shell chcon -v u:object_r:vendor_file:s0 $VendorLib/libstdc++.so"
ExecuteAdbCommand -logMsg "Setting Audio Effects Config File SEContext..." -failMsg "Failed to set Audio Effects Config File SEContext!" -command "shell chcon -v u:object_r:vendor_configs_file:s0 $VendorEtc/$AudioEffectsConfigFile"

ExecuteAdbCommand "Rebooting Device to Load Libraries..." -failMsg "Failed to Reboot Device!" -command "reboot"
Write-Output "Please wait for Device to reboot!"

$Timestamp = (Get-Date).ToString()
Write-Output "Menrva Deployed to Device at : $Timestamp"
