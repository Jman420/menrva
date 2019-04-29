$RootSourceDir = "./menrva/app/src/main"
$SourceTemplatesDir = "$RootSourceDir/templates"
$CppCommandHandlersDir = "$RootSourceDir/cpp/command_handlers"
$CppCommandHandlerMapDir = "$RootSourceDir/cpp/engine"

$CppHeaderFileExtension = ".h"
$CppClassFileExtension = ".cpp"
$TemplateCommandNameField = "<CommandName>"
$TemplateHandlerMapIncludesField = "<HandlerMapIncludes>"
$TemplateHandlerMapEntriesField = "<HandlerMapEntries>"
$CommandHandlerFileSuffix = "_Handler"
$CommandHandlerFilePattern = "*$CommandHandlerFileSuffix$CppClassFileExtension"
$CommandHandlerExcludePattern = "Module_*"

$CppHandlerMapHeaderTemplateFile = "$SourceTemplatesDir/CommandHandlerMap.h.template"
$CppHandlerMapClassTemplateFile = "$SourceTemplatesDir/CommandHandlerMap.cpp.template"
$CppHandlerMapIncludeTemplateFile = "$SourceTemplatesDir/CommandHandlerMapInclude.template"
$CppHandlerMapEntryTemplateFile = "$SourceTemplatesDir/CommandHandlerMapEntry.template"
$CppHandlerMapHeaderFile = "$CppCommandHandlerMapDir/CommandHandlerMap.h"
$CppHandlerMapClassFile = "$CppCommandHandlerMapDir/CommandHandlerMap.cpp"

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
Out-File -Force -FilePath "$CppHandlerMapHeaderFile" -InputObject $cppCommandHandlerHeader -Encoding ASCII

Write-Output "Generating CommandHandlerMap Class File..."
$cppCommandHandlerClass = $cppHandlerMapClassTemplate.Replace($TemplateHandlerMapIncludesField, $includesReplacement).Replace($TemplateHandlerMapEntriesField, $mapEntriesReplacement)
Out-File -Force -FilePath "$CppHandlerMapClassFile" -InputObject $cppCommandHandlerClass -Encoding ASCII

Write-Output "Successfully generated Command Handler Map Classes!"
