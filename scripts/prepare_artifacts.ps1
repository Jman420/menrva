param([string]$BuildType = "debug")

. ./build_variables.ps1
$MenrvaBuildApk = "$RootAppDir/build/outputs/apk/$BuildType/app-$BuildType.apk"
$AppArtifact = "$ArtifactsRootDir/MenrvaApp-$BuildType.apk"

if (!(Test-Path $MenrvaBuildApk)) {
    Write-Output "No successful build found for : $BuildType.  Complete a successful build and try again."
    exit 1
}

Write-Output "Clearing Existing Artifacts..."
if (Test-Path $ArtifactsRootDir) {
    Remove-Item $ArtifactsRootDir -Recurse -Force
}
New-Item -ItemType directory -Path $ArtifactsRootDir

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
Write-Output "Successfully extracted Artifacts into $ArtifactsRootDir !"
