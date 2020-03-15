. ./variables.ps1

Write-Output "Loading Java & C++ Template Files..."
$cppHandlerMapHeaderTemplate = Get-Content -Path $CppCommandHandlerMapHeaderTemplateFile
$cppHandlerMapClassTemplate = Get-Content -Path $CppCommandHandlerMapClassTemplateFile
$cppHandlerMapIncludeTemplate = Get-Content -Path $CppCommandHandlerMapIncludeTemplateFile
$cppHandlerMapEntryTemplate = Get-Content -Path $CppCommandHandlerMapEntryTemplateFile

Write-Output "Removing existing CommandHandlerMap files..."
if (Test-Path $CppCommandHandlerMapHeaderFile) {
    Write-Output "Removing $CppCommandHandlerMapHeaderFile..."
    Remove-Item $CppCommandHandlerMapHeaderFile -Force
}
if (Test-Path $CppCommandHandlerMapClassFile) {
    Write-Output "Removing $CppCommandHandlerMapClassFile..."
    Remove-Item $CppCommandHandlerMapClassFile -Force
}

Write-Output "Generating CommandHandlerMap Files..."
$handlerFiles = (Get-ChildItem -Path "$CppCommandHandlersSourceDir/$CommandHandlerFilePattern").Name
foreach ($handlerFile in $handlerFiles) {
    Write-Output "Initializing for Handler File : $handlerFile"
    $commandName = $handlerFile.Replace("$CommandHandlerFileSuffix$CppClassFileExtension", "")

    $includeEntry = $cppHandlerMapIncludeTemplate.Replace($TemplateCommandNameField, $commandName)
    $includesReplacement = "$includesReplacement`n" + `
                           "$includeEntry"
    
    $mapEntry = $cppHandlerMapEntryTemplate.Replace($TemplateCommandNameField, $commandName)
    $mapEntriesReplacement = "$mapEntriesReplacement`n" + `
                             "$mapEntry"
}

Write-Output "Generating CommandHandlerMap Header File..."
$cppCommandHandlerHeader = $cppHandlerMapHeaderTemplate
$cppCommandHandlerHeader = $cppCommandHandlerHeader.Replace($TemplateYearField, $CurrentYear)
Out-File -Force -FilePath "$CppCommandHandlerMapHeaderFile" -InputObject $cppCommandHandlerHeader -Encoding ASCII

Write-Output "Generating CommandHandlerMap Class File..."
$cppCommandHandlerClass = $cppHandlerMapClassTemplate
$cppCommandHandlerClass = $cppCommandHandlerClass.Replace($TemplateHandlerMapIncludesField, $includesReplacement)
$cppCommandHandlerClass = $cppCommandHandlerClass.Replace($TemplateHandlerMapEntriesField, $mapEntriesReplacement)
$cppCommandHandlerClass = $cppCommandHandlerClass.Replace($TemplateYearField, $CurrentYear)
Out-File -Force -FilePath "$CppCommandHandlerMapClassFile" -InputObject $cppCommandHandlerClass -Encoding ASCII

Write-Output "Successfully generated Command Handler Map Classes!"
