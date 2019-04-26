param([String]$commandName)
if ([String]::IsNullOrWhiteSpace($commandName)) {
    Write-Output "Must supply a Command Name as script argument."
    Write-Output "Example : ./create_new_command_handler.ps1 CommandName"
    exit 1
}

$RootSourceDir = "./menrva/app/src/main"
$SourceTemplatesDir = "$RootSourceDir/templates"
$CppOutputCommandHandlersDir = "$RootSourceDir/cpp/command_handlers"

$CppHeaderFileExtension = ".h"
$CppClassFileExtension = ".cpp"
$TemplateCommandNameField = "<CommandName>"
$CommandHandlerFileSuffix = "_Handler"
$CommandHandlerFilePattern = "*$CommandHandlerFileSuffix$CppClassFileExtension"

$CppCommandHandlerHeaderTemplateFile = "$SourceTemplatesDir/CommandHandler.h.template"
$CppCommandHandlerClassTemplateFile = "$SourceTemplatesDir/CommandHandler.cpp.template"

Write-Output "Loading Java & C++ Template Files..."
$cppCommandHandlerHeaderTemplate = Get-Content -Path $CppCommandHandlerHeaderTemplateFile
$cppCommandHandlerClassTemplate = Get-Content -Path $CppCommandHandlerClassTemplateFile

Write-Output "Creating New Command Handler : $commandName"
$cppOutputCommandHandlerHeaderFile = "$CppOutputCommandHandlersDir/$commandName$CommandHandlerFileSuffix$CppHeaderFileExtension"
$cppOutputCommandHandlerClassFile = "$CppOutputCommandHandlersDir/$commandName$CommandHandlerFileSuffix$CppClassFileExtension"
if (Test-Path $cppOutputCommandHandlerClassFile) {
    Write-Output "Command Handler already exists.  Delete the following files to regenerate them :"
    Write-Output "    - $cppOutputCommandHandlerHeaderFile"
    Write-Output "    - $cppOutputCommandHandlerClassFile"
    exit 1
}

Write-Output "Generating Command Handler : $commandName"
$cppCommandHandlerHeaderFile = $cppCommandHandlerHeaderTemplate.Replace($TemplateCommandNameField, $commandName)
$cppCommandHandlerClassFile = $cppCommandHandlerClassTemplate.Replace($TemplateCommandNameField, $commandName)
Out-File -Force -FilePath "$cppOutputCommandHandlerHeaderFile" -InputObject $cppCommandHandlerHeaderFile -Encoding ASCII
Out-File -Force -FilePath "$cppOutputCommandHandlerClassFile" -InputObject $cppCommandHandlerClassFile -Encoding ASCII

& "$PSScriptRoot\generate_command_handler_base.ps1"

& "$PSScriptRoot\generate_command_handler_map.ps1"
