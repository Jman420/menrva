. ./variables.ps1

Write-Output "Loading Java & C++ Template Files..."
$cppHandlerMapHeaderTemplate = Get-Content -Path $CppHandlerMapHeaderTemplateFile
$cppHandlerMapClassTemplate = Get-Content -Path $CppHandlerMapClassTemplateFile
$cppHandlerMapIncludeTemplate = Get-Content -Path $CppHandlerMapIncludeTemplateFile
$cppHandlerMapEntryTemplate = Get-Content -Path $CppHandlerMapEntryTemplateFile

Write-Output "Removing existing CommandHandlerMap files..."
if (Test-Path $CppHandlerMapHeaderFile) {
    Write-Output "Removing $CppHandlerMapHeaderFile..."
    Remove-Item $CppHandlerMapHeaderFile -Force
}
if (Test-Path $CppHandlerMapClassFile) {
    Write-Output "Removing $CppHandlerMapClassFile..."
    Remove-Item $CppHandlerMapClassFile -Force
}

Write-Output "Generating CommandHandlerMap Files..."
$handlerFiles = (Get-ChildItem -Path "$CppCommandHandlersDir/$CommandHandlerFilePattern" -Exclude $CommandHandlerExcludePattern).Name
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
Out-File -Force -FilePath "$CppHandlerMapHeaderFile" -InputObject $cppCommandHandlerHeader -Encoding ASCII

Write-Output "Generating CommandHandlerMap Class File..."
$cppCommandHandlerClass = $cppHandlerMapClassTemplate
$cppCommandHandlerClass = $cppCommandHandlerClass.Replace($TemplateHandlerMapIncludesField, $includesReplacement)
$cppCommandHandlerClass = $cppCommandHandlerClass.Replace($TemplateHandlerMapEntriesField, $mapEntriesReplacement)
$cppCommandHandlerClass = $cppCommandHandlerClass.Replace($TemplateYearField, $CurrentYear)
Out-File -Force -FilePath "$CppHandlerMapClassFile" -InputObject $cppCommandHandlerClass -Encoding ASCII

Write-Output "Successfully generated Command Handler Map Classes!"
