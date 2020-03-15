param([String]$CommandName)
if ([String]::IsNullOrWhiteSpace($CommandName)) {
    Write-Output "Must supply a Command Name as script argument."
    Write-Output "Example : ./create_new_command_handler.ps1 [CommandName]"
    exit 1
}

. ./variables.ps1

Write-Output "Loading C++ Template Files..."
$cppCommandHandlerHeaderTemplate = Get-Content -Path $CppCommandHandlerHeaderTemplateFile
$cppCommandHandlerClassTemplate = Get-Content -Path $CppCommandHandlerClassTemplateFile

Write-Output "Creating New Command Handler : $CommandName"
$cppOutputCommandHandlerHeaderFile = "$CppCommandHandlersSourceDir/$CommandName$CommandHandlerFileSuffix$CppHeaderFileExtension"
$cppOutputCommandHandlerClassFile = "$CppCommandHandlersSourceDir/$CommandName$CommandHandlerFileSuffix$CppClassFileExtension"
if (Test-Path $cppOutputCommandHandlerHeaderFile -Or Test-Path $cppOutputCommandHandlerClassFile) {
    Write-Output "Command Handler already exists.  Delete the following files to regenerate them :"
    Write-Output "    - $cppOutputCommandHandlerHeaderFile"
    Write-Output "    - $cppOutputCommandHandlerClassFile"
    exit 1
}

Write-Output "Generating Command Handler : $CommandName"
$cppCommandHandlerHeaderFile = $cppCommandHandlerHeaderTemplate
$cppCommandHandlerHeaderFile = $cppCommandHandlerHeaderFile.Replace($TemplateCommandNameField, $CommandName)
$cppCommandHandlerHeaderFile = $cppCommandHandlerHeaderFile.Replace($TemplateYearField, $CurrentYear)
Out-File -Force -FilePath "$cppOutputCommandHandlerHeaderFile" -InputObject $cppCommandHandlerHeaderFile -Encoding ASCII

$cppCommandHandlerClassFile = $cppCommandHandlerClassTemplate
$cppCommandHandlerClassFile = $cppCommandHandlerClassFile.Replace($TemplateCommandNameField, $CommandName)
$cppCommandHandlerClassFile = $cppCommandHandlerClassFile.Replace($TemplateYearField, $CurrentYear)
Out-File -Force -FilePath "$cppOutputCommandHandlerClassFile" -InputObject $cppCommandHandlerClassFile -Encoding ASCII

& "$PSScriptRoot\generate_command_handler_base.ps1"

& "$PSScriptRoot\generate_command_handler_map.ps1"

Write-Output "Successfully generated Command Handler : $cppOutputCommandHandlerClassFile !"
