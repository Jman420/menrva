. ./variables.ps1

$RepoUrl = "https://github.com/Zackptg5/MMT-Extended/archive/master.zip"
$RepoZipFile = "$RepoLibsDir/MMT-Extended-master.zip"

Write-Output "Preparing Magisk Module Template Directory..."
if (Test-Path $RepoZipFile) {
    Write-Output "Removing existing MMT-Extended Repo Zip File..."
    Remove-Item $RepoZipFile -Force
}
Write-Output "Downloading MMT-Extended Repo Zip File..."
Start-BitsTransfer -Source $RepoUrl -Destination $RepoZipFile

if (Test-Path $MagiskModuleDir) {
    Write-Output "Removing Magisk Module Template Directory..."
    Remove-Item $MagiskModuleDir -Recurse -Force
}
Write-Output "Unzipping MMT-Extended to Magisk Module Template Directory..."
7z x "$RepoZipFile" -o"$RepoLibsDir" -r
Write-Output "Successfully prepared Magisk Module Template!"
