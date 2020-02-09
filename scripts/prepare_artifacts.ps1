param([bool]$IsOptimizedBuild = $true, [bool]$IsReleaseBuild = $true, [bool]$IsSigned = $false)

. ./variables.ps1
$productFlavor = "optimized"
if (-Not $IsOptimizedBuild) {
    $productFlavor = "unoptimized"
}
$buildType = "release"
if (-Not $IsReleaseBuild) {
    $buildType = "debug"
}
$apkSuffix = ""
if (-Not $IsSigned -And $IsReleaseBuild) {
    $apkSuffix = "-unsigned"
}

$MenrvaBuildApk = "$RootAppDir/build/outputs/apk/$productFlavor/$buildType/app-$productFlavor-$buildType$apkSuffix.apk"

if (!(Test-Path $MenrvaBuildApk)) {
    Write-Output "No successful build found for : $productFlavor $buildType.  Complete a successful build and try again."
    exit 1
}

if (Test-Path $ArtifactsRootDir) {
    Write-Output "Clearing Existing Artifacts..."
    Remove-Item $ArtifactsRootDir -Recurse -Force
}
New-Item -ItemType directory -Path $ArtifactsRootDir

Write-Output "Copying Menrva App APK to Artifacts Directory..."
Copy-Item -Path $MenrvaBuildApk -Destination $AppArtifact

Write-Output "Extracting Backend from Menrva App APK..."
7z x $MenrvaBuildApk lib/* -o"$ArtifactsRootDir"

Write-Output "Writing Build Information to Artifacts Info File..."
$InfoFileContents = "Product Flavor : $productFlavor`n" + 
                    "Build Type : $buildType`n" + 
                    "APK Suffix : $apkSuffix"
Set-Content -Path $ArtifactsRootDir/info.txt -Value $InfoFileContents

Write-Output "Successfully prepared Artifacts into $ArtifactsRootDir !"
