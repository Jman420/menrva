. ./variables.ps1

Write-Output "Removing Output Directories..."
if (Test-Path $JavaCommandOutputDir) {
    Write-Output "Removing Java Output Directory : $JavaCommandOutputDir ..."
    Remove-Item $JavaCommandOutputDir -Force -Recurse
}

Write-Output "Creating Java Output Directories..."
New-Item -ItemType directory -Force -Path $JavaCommandOutputDir
New-Item -ItemType directory -Force -Path $JavaMessageOutputDir

Write-Output "Loading Java Template Files..."
$javaCommandClassTemplate = Get-Content -Path $JavaCommandClassTemplateFile
$javaCommandEnumTemplate = Get-Content -Path $JavaCommandEnumTemplateFile
$javaCommandEnumEntryTemplate = Get-Content -Path $JavaCommandEnumEntryTemplateFile

Write-Output "Generating Protobuf Message & Command Files..."
$protoPath = Resolve-Path -Path $ProtobufSourceDir -Relative
$protobufFiles = (Get-ChildItem -Path "$ProtobufSourceDir/$ProtobufFilePattern").FullName | Resolve-Path -Relative
foreach ($protoFile in $protobufFiles) {
  Write-Output "Initializing for Protobuf File : $protoFile"
  $commandName = (Split-Path $protoFile -Leaf).Replace($ProtobufFileExtension, "")
  $commandIdLine = (Select-String -Path $protoFile -Pattern $CommandIdPrefixPattern).Line
  $commandIdDelimiterIndex = $commandIdLine.IndexOf($CommandIdDelimiter)
  $commandId = $commandIdLine.Substring($commandIdDelimiterIndex + 1)
  $javaCommandFileName = $commandName + $CommandClassFileSuffix + $JavaFileExtension

  Write-Output "Compiling Protobuf File : $protoFile"
  . $ProtocExe `
    --proto_path="$protoPath" `
    --java_out="lite:$JavaSourceDir" `
    "$protoFile"
  
  Write-Output "Generating Java Command File : $JavaCommandOutputDir/$javaCommandFileName"
  $javaCommandFile = $javaCommandClassTemplate
  $javaCommandFile = $javaCommandFile.Replace($TemplateCommandNameField, $commandName)
  $javaCommandFile = $javaCommandFile.Replace($TemplateYearField, $CurrentYear)
  Out-File -Force -FilePath "$JavaCommandOutputDir/$javaCommandFileName" -InputObject $javaCommandFile -Encoding ASCII
  
  Write-Output "Adding Entry to Java Command Enum for : $commandName"
  $javaEnumCommandEntry = $javaCommandEnumEntryTemplate
  $javaEnumCommandEntry = $javaEnumCommandEntry.Replace($TemplateCommandNameField, $commandName)
  $javaEnumCommandEntry = $javaEnumCommandEntry.Replace($TemplateCommandIdField, $commandId)
  $javaEnumEntryReplacement = "$javaEnumEntryReplacement`n" + `
                              "$javaEnumCommandEntry"
}

Write-Output "Generating Java Command Enum File : $JavaCommandEnumFile"
$javaEnumEntryReplacement = $javaEnumEntryReplacement.Replace("`n$TemplateCommandEnumEntryField", "")
$javaCommandEnum = $javaCommandEnumTemplate
$javaCommandEnum = $javaCommandEnum.Replace($TemplateCommandEnumEntryField, $javaEnumEntryReplacement)
$javaCommandEnum = $javaCommandEnum.Replace($TemplateYearField, $CurrentYear)
Out-File -Force -FilePath "$JavaCommandEnumFile" -InputObject $javaCommandEnum -Encoding ASCII
Write-Output "Successfully Generated Protobuf Message & Command Files!"
