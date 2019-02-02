param([string]$Architecture = "x86",
      [string]$BuildType = "debug")

$MenrvaBuildApk = "./menrva/app/build/outputs/apk/$BuildType/app-$BuildType.apk"
$ArtifactsRoot = "./artifacts"
$AppArtifact = "$ArtifactsRoot/MenrvaApp-$BuildType.apk"
$BackendArtifactDir = "$ArtifactsRoot/backend"
      
$AdbExe = "$env:LOCALAPPDATA/Android/Sdk/platform-tools/adb.exe"
$MenrvaLib = "$BackendArtifactDir/$Architecture/libMenrvaEngine.so"
$Fftw3Lib = "$BackendArtifactDir/$Architecture/libfftw3.so"
$KissFftLib = "$BackendArtifactDir/$Architecture/libkissfft.so"
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

Write-Output "Clearing Existing Artifacts..."
if (Test-Path -Path $ArtifactsRoot) {
    Remove-Item $ArtifactsRoot -Recurse -Force
}
New-Item -ItemType directory -Path $ArtifactsRoot

Write-Output "Copying Menrva App APK to Artifacts Directory..."
Copy-Item -Path $MenrvaBuildApk -Destination $AppArtifact

Write-Output "Extracting Backend from Menrva App APK..."
Add-Type -Assembly System.IO.Compression.FileSystem
$apkZip = [IO.Compression.ZipFile]::OpenRead("../$AppArtifact")
$backendFileEntries = $apkZip.Entries | where { $_.FullName -like "lib/*" }
foreach ($backendFileEntry in $backendFileEntries) {
    $backendOutputFile = $backendFileEntry.FullName.Replace("lib/", "")
    New-Item -Force "$BackendArtifactDir/$backendOutputFile"
    [IO.Compression.ZipFileExtensions]::ExtractToFile($backendFileEntry, "../$BackendArtifactDir/$backendOutputFile", $true) 
}
$apkZip.Dispose()

ExecuteAdbCommand -logMsg "Installing Menrva App APK..." -failMsg "Failed to Install Menrva App APK!" -command "install -r $AppArtifact"
ExecuteAdbCommand -logMsg "Obtaining Root on Device..." -failMsg "Failed to Obtain Root on Device!" -command "root"
ExecuteAdbCommand -logMsg "Remounting Device Volumes..." -failMsg "Failed to Remount Device Volumes!" -command "remount"
ExecuteAdbCommand -logMsg "Pushing Menrva Lib to Device..." -failMsg "Failed to Push Menrva Lib to Device!" -command "push $MenrvaLib $SoundFxLib"
ExecuteAdbCommand -logMsg "Pushing FFTW Lib to Device..." -failMsg "Failed to Push FFTW Lib to Device!" -command "push $Fftw3Lib $VendorLib"
ExecuteAdbCommand -logMsg "Pushing KissFFT Lib to Device..." -failMsg "Failed to Push KissFFT Lib to Device!" -command "push $KissFftLib $VendorLib"
ExecuteAdbCommand -logMsg "Pushing Audio Effects Config to Device..." -failMsg "Failed to Push Audio Effects Config to Device!" -command "push $AudioEffectsConfig $VendorEtc"
ExecuteAdbCommand "Rebooting Device to Load Libraries..." -failMsg "Failed to Reboot Device!" -command "reboot"
Write-Output "Please wait for Device to reboot!"

$Timestamp = (Get-Date).ToString()
Write-Output "Menrva Deployed to Device at : $Timestamp"
