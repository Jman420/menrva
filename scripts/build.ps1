param([bool]$IsOptimizedBuild = $true, [bool]$IsReleaseBuild = $true)

. ./variables.ps1

$command = "assemble"
$productFlavor = "Optimized"
if (-Not $IsOptimizedBuild) {
    $productFlavor = "Unoptimized"
}
$buildType = "Release"
if (-Not $IsReleaseBuild) {
    $buildType = "Debug"
}

Write-Output "Executing $command$productFlavor$buildType ..."
Push-Location $RepoProjectDir
./gradlew.bat "$command$productFlavor$buildType"
Pop-Location
Write-Output "Successfully executed $command$productFlavor$buildType !"
