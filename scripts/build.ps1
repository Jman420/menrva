param([bool]$IsDebugBuild = $true)

. ./variables.ps1

$command = "assemble"
if ($IsDebugBuild) {
    $command = "assembleDebug"
}

Push-Location $RepoProjectDir
./gradlew.bat ":app:$command"

Pop-Location
