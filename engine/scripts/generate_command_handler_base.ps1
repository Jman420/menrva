. ./variables.ps1

Write-Output "Loading C++ Template Files..."
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
$handlerFiles = (Get-ChildItem -Path "$CppCommandHandlersSourceDir/$CommandHandlerFilePattern").Name
foreach ($handlerFile in $handlerFiles) {
    Write-Output "Initializing for Handler File : $handlerFile"
    $handlerCommandName = $handlerFile.Replace("$CommandHandlerFileSuffix$CppClassFileExtension", "")
    
    $typeDefEntry = $cppTypedCommandHandlerBaseTypeDefTemplate.Replace($TemplateCommandNameField, $handlerCommandName)
    $cppTypedCommandHandlerBaseTypeDefReplacement = "$cppTypedCommandHandlerBaseTypeDefReplacement`n" + `
                                                    "`n" + `
                                                    "$typeDefEntry"
}

Write-Output "Generating TypedCommandHandlerBase Header File..."
$cppTypedCommandHandlerBaseHeader = $cppTypedCommandHandlerBaseHeaderTemplate
$cppTypedCommandHandlerBaseHeader = $cppTypedCommandHandlerBaseHeader.Replace($TemplateCommandNameField, $commandName)
$cppTypedCommandHandlerBaseHeader = $cppTypedCommandHandlerBaseHeader.Replace($TemplateYearField, $CurrentYear)
Out-File -Force -FilePath "$CppTypedCommandHandlerBaseHeaderFile" -InputObject $cppTypedCommandHandlerBaseHeader -Encoding ASCII

Write-Output "Generating TypedCommandHandlerBase Class File..."
$cppTypedCommandHandlerBaseClass = $cppTypedCommandHandlerBaseClassTemplate
$cppTypedCommandHandlerBaseClass = $cppTypedCommandHandlerBaseClass.Replace($TemplateCommandNameField, $commandName)
$cppTypedCommandHandlerBaseClass = $cppTypedCommandHandlerBaseClass.Replace($TemplateCommandHandlerBaseTypeDef, $cppTypedCommandHandlerBaseTypeDefReplacement)
$cppTypedCommandHandlerBaseClass = $cppTypedCommandHandlerBaseClass.Replace($TemplateYearField, $CurrentYear)
Out-File -Force -FilePath "$CppTypedCommandHandlerBaseClassFile" -InputObject $cppTypedCommandHandlerBaseClass -Encoding ASCII

Write-Output "Successfully generated Command Handler Base Classes!"
