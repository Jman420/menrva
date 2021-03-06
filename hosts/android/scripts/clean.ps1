. ./variables.ps1

$AppCxxPath = "$AndroidAppDir/.cxx"
$AppBuildPath = "$AndroidAppDir/build"

function Remove-Dir {
    param( [string]$targetDir )
    
    if (Test-Path $targetDir) {
        Write-Output "Removing $targetDir ..."
        Remove-Item $targetDir -Recurse -Force
    }
}

Write-Output "Cleaning Build Directories..."
Remove-Dir $AppCxxPath
Remove-Dir $AppBuildPath

Remove-Dir $MagiskModuleDir

Remove-Dir $ArtifactsRootDir
Write-Output "Successfully Cleaned Build Directories!"
