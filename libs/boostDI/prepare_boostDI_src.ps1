$RepoUrl = "https://github.com/boost-experimental/di/archive/cpp14.zip"
$RepoZipFile = "./boostDI-cpp14.zip"
$RootZipFolder = "di-cpp14"
$RootSourcePath = "./src"

Write-Output "Preparing Boost.DI Source Code Directory..."
if (Test-Path $RepoZipFile) {
    Write-Output "Removing existing Boost.DI Repo Zip File..."
    Remove-Item $RepoZipFile -Force
}
Write-Output "Downloading Boost.DI Repo Zip File..."
Start-BitsTransfer -Source $RepoUrl -Destination $RepoZipFile

if (Test-Path $RootSourcePath) {
    Write-Output "Removing Boost.DI Source Code Directory..."
    Remove-Item $RootSourcePath -Recurse -Force
}
Write-Output "Unzipping Boost.DI Repo to Boost.DI Source Code Directory..."
7z x "$RepoZipFile" -r
mv ./$RootZipFolder $RootSourcePath
Write-Output "Successfully prepared Boost.DI Source Code!"
