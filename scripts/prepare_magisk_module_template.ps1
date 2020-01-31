. ./build_variables.ps1

$RepoUrl = "https://github.com/Zackptg5/MMT-Extended/archive/master.zip"
$RepoZipFile = "$RepoLibsDir/MMT-Extended-master.zip"

if (Test-Path $RepoZipFile) {
    Remove-Item $RepoZipFile -Force
}
Start-BitsTransfer -Source $RepoUrl -Destination $RepoZipFile

if (Test-Path $MagiskModuleDir) {
    Remove-Item $MagiskModuleDir -Recurse -Force
}
7z x "$RepoZipFile" -o"$RepoLibsDir" -r
