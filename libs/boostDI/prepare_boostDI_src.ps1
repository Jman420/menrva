$RepoUrl = "https://github.com/boost-experimental/di/archive/cpp14.zip"
$RepoZipFile = "./boostDI-cpp14.zip"
$RootZipFolder = "di-cpp14"
$SourceIncludeFolders = @("include", "extension/include/boost")
$RootSourcePath = "include"

Write-Output "Preparing Boost.DI Source Code Directory..."
if (Test-Path $RepoZipFile) {
    Write-Output "Removing existing Boost.DI Repo Zip File..."
    Remove-Item $RepoZipFile -Force
}
Write-Output "Downloading Boost.DI Repo Zip File..."
Invoke-WebRequest -Uri $RepoUrl -OutFile $RepoZipFile

Write-Output "Unzipping Boost.DI Repo..."
7z x "$RepoZipFile" -r

if (Test-Path $RootSourcePath) {
    Write-Output "Removing Boost.DI Source Code Directory..."
    Remove-Item $RootSourcePath -Recurse -Force
}
foreach ($includeFolder in $SourceIncludeFolders) {
    Write-Output "Moving Source Directory to Include Directory : $includeFolder ..."
    Copy-Item -Path ./$RootZipFolder/$includeFolder -Destination $RootSourcePath -Force -Recurse
}
Remove-Item $RootZipFolder -Recurse -Force
Write-Output "Successfully prepared Boost.DI Source Code!"
