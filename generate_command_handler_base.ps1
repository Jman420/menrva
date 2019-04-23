$RootSourceDir = "./menrva/app/src/main"
$SourceTemplatesDir = "$RootSourceDir/templates"
$CppOutputCommandHandlersDir = "$RootSourceDir/cpp/command_handlers"

$CppClassFileExtension = ".cpp"
$TemplateCommandNameField = "<CommandName>"
$TemplateCommandHandlerBaseTypeDef = "<CommandHandlerBaseTypeDef>"
$CommandHandlerFileSuffix = "_Handler"
$CommandHandlerFilePattern = "*$CommandHandlerFileSuffix$CppClassFileExtension"

$CppTypedCommandHandlerBaseHeaderTemplateFile = "$SourceTemplatesDir/TypedCommandHandlerBase.h.template"
$CppTypedCommandHandlerBaseClassTemplateFile = "$SourceTemplatesDir/TypedCommandHandlerBase.cpp.template"
$CppTypedCommandHandlerBaseTypeDefTemplateFile = "$SourceTemplatesDir/TypedCommandHandlerBaseTypes.template"
$CppTypedCommandHandlerBaseHeaderFile = "$CppOutputCommandHandlersDir/TypedCommandHandlerBase.h"
$CppTypedCommandHandlerBaseClassFile = "$CppOutputCommandHandlersDir/TypedCommandHandlerBase.cpp"

Write-Output "Loading Java & C++ Template Files..."
$cppTypedCommandHandlerBaseHeaderTemplate = Get-Content -Path $CppTypedCommandHandlerBaseHeaderTemplateFile
$cppTypedCommandHandlerBaseClassTemplate = Get-Content -Path $CppTypedCommandHandlerBaseClassTemplateFile
$cppTypedCommandHandlerBaseTypeDefTemplate = (Get-Content -Path $CppTypedCommandHandlerBaseTypeDefTemplateFile) -Join "`n"

Write-Output "Removing existing TypedCommandHandlerBase Files..."
if (Test-Path $CppTypedCommandHandlerBaseHeaderFile) {
    Write-Output "Removing $CppTypedCommandHandlerBaseHeaderFile"
    Remove-Item $CppTypedCommandHandlerBaseHeaderFile -Force
}
if (Test-Path $CppTypedCommandHandlerBaseClassFile) {
    Write-Output "Removing $CppTypedCommandHandlerBaseClassFile"
    Remove-Item $CppTypedCommandHandlerBaseClassFile -Force
}

Write-Output "Generating TypedCommandHandlerBase Files..."
$handlerFiles = (Get-ChildItem -Path "$CppOutputCommandHandlersDir/$CommandHandlerFilePattern").Name
foreach ($handlerFile in $handlerFiles) {
    Write-Output "Initializing for Handler File : $handlerFile"
    $handlerCommandName = $handlerFile.Replace("$CommandHandlerFileSuffix$CppClassFileExtension", "")
    $typeDefEntry = $cppTypedCommandHandlerBaseTypeDefTemplate.Replace($TemplateCommandNameField, $handlerCommandName)
    $cppTypedCommandHandlerBaseTypeDefReplacement = "$cppTypedCommandHandlerBaseTypeDefReplacement`n" + `
                                                    "`n" + `
                                                    "$typeDefEntry"
}

Write-Output "Generating TypedCommandHandlerBase Header File..."
$cppTypedCommandHandlerBaseHeader = $CppTypedCommandHandlerBaseHeaderTemplate.Replace($TemplateCommandNameField, $commandName)
Out-File -Force -FilePath "$CppTypedCommandHandlerBaseHeaderFile" -InputObject $cppTypedCommandHandlerBaseHeader -Encoding ASCII

Write-Output "Generating TypedCommandHandlerBase Class File..."
$cppTypedCommandHandlerBaseClass = $CppTypedCommandHandlerBaseClassTemplate.Replace($TemplateCommandNameField, $commandName).Replace($TemplateCommandHandlerBaseTypeDef, $cppTypedCommandHandlerBaseTypeDefReplacement)
Out-File -Force -FilePath "$CppTypedCommandHandlerBaseClassFile" -InputObject $cppTypedCommandHandlerBaseClass -Encoding ASCII
