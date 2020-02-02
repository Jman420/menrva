param([String]$CommandName)
if ([String]::IsNullOrWhiteSpace($CommandName)) {
    Write-Output "Must supply a Command Name as script argument."
    Write-Output "Example : ./create_new_command_handler.ps1 [CommandName]"
    exit 1
}

. ./variables.ps1
$CppProjectCacheDir = "$RootAppDir/.cxx"

Write-Output "Loading Java & C++ Template Files..."
$cppCommandHandlerHeaderTemplate = Get-Content -Path $CppCommandHandlerHeaderTemplateFile
$cppCommandHandlerClassTemplate = Get-Content -Path $CppCommandHandlerClassTemplateFile

Write-Output "Creating New Command Handler : $CommandName"
$cppOutputCommandHandlerHeaderFile = "$CppOutputCommandHandlersDir/$CommandName$CommandHandlerFileSuffix$CppHeaderFileExtension"
$cppOutputCommandHandlerClassFile = "$CppOutputCommandHandlersDir/$CommandName$CommandHandlerFileSuffix$CppClassFileExtension"
if (Test-Path $cppOutputCommandHandlerClassFile) {
    Write-Output "Command Handler already exists.  Delete the following files to regenerate them :"
    Write-Output "    - $cppOutputCommandHandlerHeaderFile"
    Write-Output "    - $cppOutputCommandHandlerClassFile"
    exit 1
}

Write-Output "Generating Command Handler : $CommandName"
$cppCommandHandlerHeaderFile = $cppCommandHandlerHeaderTemplate.Replace($TemplateCommandNameField, $CommandName)
$cppCommandHandlerClassFile = $cppCommandHandlerClassTemplate.Replace($TemplateCommandNameField, $CommandName)
Out-File -Force -FilePath "$cppOutputCommandHandlerHeaderFile" -InputObject $cppCommandHandlerHeaderFile -Encoding ASCII
Out-File -Force -FilePath "$cppOutputCommandHandlerClassFile" -InputObject $cppCommandHandlerClassFile -Encoding ASCII

& "$PSScriptRoot\generate_command_handler_base.ps1"

& "$PSScriptRoot\generate_command_handler_map.ps1"

if (Test-Path $CppProjectCacheDir) {
    Write-Output "Clearing C++ Project Cache to include New Command Handler..."
    Remove-Item "$CppProjectCacheDir" -Recurse -Force
}

Write-Output "Successfully generated Command Handler : $cppOutputCommandHandlerClassFile !"
