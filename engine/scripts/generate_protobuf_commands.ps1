. ./variables.ps1

Write-Output "Removing Output Directories..."
Write-Output "Removing C++ Output Directory contents from : $CppCommandSourceDir ..."
Remove-Item $CppCommandSourceDir\* -Force -Recurse -Exclude "CommandBase*"

Write-Output "Creating C++ Output Directories..."
New-Item -ItemType directory -Force -Path $CppCommandSourceDir
New-Item -ItemType directory -Force -Path $CppMessageSourceDir

Write-Output "Loading C++ Template Files..."
$cppCommandHeaderTemplate = Get-Content -Path $CppCommandHeaderTemplateFile
$cppCommandClassTemplate = Get-Content -Path $CppCommandClassTemplateFile
$cppCommandEnumTemplate = Get-Content -Path $CppCommandEnumTemplateFile
$cppCommandEnumEntryTemplate = Get-Content -Path $CppCommandEnumEntryTemplateFile
$cppCommandBaseTypeDefTemplate = (Get-Content -Path $CppCommandBaseTypeDefTemplateFile) -Join "`n"
$cppCommandBaseHeaderTemplate = Get-Content -Path $CppCommandBaseHeaderTemplateFile
$cppCommandBaseClassTemplate = Get-Content -Path $CppCommandBaseClassTemplateFile

Write-Output "Generating C++ Protobuf Message & Command Files..."
$protoPath = Resolve-Path -Path $ProtobufSourceDir -Relative
$protobufFiles = (Get-ChildItem -Path "$ProtobufSourceDir/$ProtobufFilePattern").FullName | Resolve-Path -Relative
foreach ($protoFile in $protobufFiles) {
  Write-Output "Initializing for Protobuf File : $protoFile"
  $commandName = (Split-Path $protoFile -Leaf).Replace($ProtobufFileExtension, "")
  $commandIdLine = (Select-String -Path $protoFile -Pattern "CommandId=").Line
  $commandIdDelimiterIndex = $commandIdLine.IndexOf("=")
  $commandId = $commandIdLine.Substring($commandIdDelimiterIndex + 1)
  $cppCommandHeaderFileName = $commandName + $CommandClassFileSuffix + $CppHeaderFileExtension
  $cppCommandClassFileName = $commandName + $CommandClassFileSuffix + $CppClassFileExtension

  Write-Output "Compiling Protobuf File : $protoFile"
  . $ProtocExe `
    --proto_path="$protoPath" `
    --cpp_out="lite:$CppMessageSourceDir" `
    "$protoFile"
  
  Write-Output "Generating C++ Command Header File : $CppCommandSourceDir/$cppCommandHeaderFileName"
  $cppCommandHeaderFile = $cppCommandHeaderTemplate
  $cppCommandHeaderFile = $cppCommandHeaderFile.Replace($TemplateCommandNameField, $commandName)
  $cppCommandHeaderFile = $cppCommandHeaderFile.Replace($TemplateYearField, $CurrentYear)
  Out-File -Force -FilePath "$CppCommandSourceDir/$cppCommandHeaderFileName" -InputObject $cppCommandHeaderFile -Encoding ASCII
  
  Write-Output "Generating C++ Command Class File : $CppCommandSourceDir/$cppCommandClassFileName"
  $cppCommandClassFile = $cppCommandClassTemplate
  $cppCommandClassFile = $cppCommandClassFile.Replace($TemplateCommandNameField, $commandName)
  $cppCommandClassFile = $cppCommandClassFile.Replace($TemplateCommandEnumFileField, $CommandEnumFileName)
  $cppCommandClassFile = $cppCommandClassFile.Replace($TemplateYearField, $CurrentYear)
  Out-File -Force -FilePath "$CppCommandSourceDir/$cppCommandClassFileName" -InputObject $cppCommandClassFile -Encoding ASCII
  
  Write-Output "Adding Entry to C++ Command Enum for : $commandName"
  $cppEnumCommandEntry = $cppCommandEnumEntryTemplate
  $cppEnumCommandEntry = $cppEnumCommandEntry.Replace($TemplateCommandNameField, $commandName)
  $cppEnumCommandEntry = $cppEnumCommandEntry.Replace($TemplateCommandIdField, $commandId)
  $cppEnumEntryReplacement = "$cppEnumEntryReplacement`n" +
                             "$cppEnumCommandEntry"
  
  Write-Output "Adding Entry to C++ Command Base TypeDef for : $commandName"
  $typeDefEntry = $cppCommandBaseTypeDefTemplate.Replace($TemplateCommandNameField, $commandName)
  $cppCommandBaseTypeDefReplacement = "$cppCommandBaseTypeDefReplacement`n" + `
                                      "`n" + `
                                      "$typeDefEntry"
}

Write-Output "Generating C++ Command Enum File : $CppCommandEnumFile"
$cppEnumEntryReplacement = $cppEnumEntryReplacement.Replace("`n$TemplateCommandEnumEntryField", "")
$cppCommandEnum = $cppCommandEnumTemplate
$cppCommandEnum = $cppCommandEnum.Replace($TemplateCommandEnumEntryField, $cppEnumEntryReplacement)
$cppCommandEnum = $cppCommandEnum.Replace($TemplateYearField, $CurrentYear)
Out-File -Force -FilePath "$CppCommandEnumFile" -InputObject $cppCommandEnum -Encoding ASCII

Write-Output "Generating C++ Command Base Header File : $CppCommandBaseHeaderFile"
$cppCommandBaseHeader = $cppCommandBaseHeaderTemplate
$cppCommandBaseHeader = $cppCommandBaseHeader.Replace($TemplateCommandNameField, $commandName)
$cppCommandBaseHeader = $cppCommandBaseHeader.Replace($TemplateYearField, $CurrentYear)
Out-File -Force -FilePath "$CppCommandBaseHeaderFile" -InputObject $cppCommandBaseHeader -Encoding ASCII

Write-Output "Generating C++ Command Base Class File : $CppCommandBaseClassFile"
$cppCommandBaseTypeDefReplacement = $cppCommandBaseTypeDefReplacement.Replace("`n`n$TemplateCommandBaseTypeDefField", "")
$cppCommandBaseClass = $cppCommandBaseClassTemplate
$cppCommandBaseClass = $cppCommandBaseClass.Replace($TemplateCommandBaseTypeDefField, $cppCommandBaseTypeDefReplacement)
$cppCommandBaseClass = $cppCommandBaseClass.Replace($TemplateYearField, $CurrentYear)
Out-File -Force -FilePath "$CppCommandBaseClassFile" -InputObject $cppCommandBaseClass -Encoding ASCII

Write-Output "Successfully Generated C++ Protobuf Message & Command Files!"
