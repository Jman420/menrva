$RepoUrl = "https://raw.githubusercontent.com/boost-experimental/di/cpp14/include/boost/di.hpp"
$IncludeDir = "./include"
$OutDir = "$IncludeDir/boost"
$OutFile = "$OutDir/di.hpp"

Write-Output "Preparing Boost.DI Source Code Directory..."
if (Test-Path $IncludeDir) {
    Write-Output "Removing existing Boost.DI Source Code Directory..."
    Remove-Item $IncludeDir -Force
}
New-Item -ItemType directory -Force -Path $OutDir

Write-Output "Downloading Boost.DI Source Code..."
Start-BitsTransfer -Source $RepoUrl -Destination $OutFile
Write-Output "Successfully prepared Boost.DI Source Code!"
