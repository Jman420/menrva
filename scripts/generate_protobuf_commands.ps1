. ./build_variables.ps1

Write-Output "Removing Output Directories..."
if (Test-Path $CppOutputCommandDir) {
    Write-Output "Removing C++ Output Directory : $CppOutputCommandDir ..."
    Remove-Item $CppOutputCommandDir -Force -Recurse
}
if (Test-Path $JavaOutputCommandDir) {
    Write-Output "Removing Java Output Directory : $JavaOutputCommandDir ..."
    Remove-Item $JavaOutputCommandDir -Force -Recurse
}

Write-Output "Creating C++ Output Directories..."
New-Item -ItemType directory -Force -Path $CppOutputCommandDir
New-Item -ItemType directory -Force -Path $CppOutputMessageDir
Write-Output "Creating Java Output Directories..."
New-Item -ItemType directory -Force -Path $JavaOutputCommandDir
New-Item -ItemType directory -Force -Path $JavaOutputMessageDir

Write-Output "Loading Java & C++ Template Files..."
$javaCommandClassTemplate = Get-Content -Path $JavaCommandClassTemplateFile
$javaCommandEnumTemplate = Get-Content -Path $JavaCommandEnumTemplateFile
$javaCommandEnumEntryTemplate = Get-Content -Path $JavaCommandEnumEntryTemplateFile

$cppCommandHeaderTemplate = Get-Content -Path $CppCommandHeaderTemplateFile
$cppCommandClassTemplate = Get-Content -Path $CppCommandClassTemplateFile
$cppCommandEnumTemplate = Get-Content -Path $CppCommandEnumTemplateFile
$cppCommandEnumEntryTemplate = Get-Content -Path $CppCommandEnumEntryTemplateFile
$cppCommandBaseTypeDefTemplate = (Get-Content -Path $CppCommandBaseTypeDefTemplateFile) -Join "`n"
$cppCommandBaseHeaderTemplate = Get-Content -Path $CppCommandBaseHeaderTemplateFile
$cppCommandBaseClassTemplate = Get-Content -Path $CppCommandBaseClassTemplateFile

Write-Output "Compiling Protobuf Message & Command Files..."
$protobufFiles = (Get-ChildItem -Path "$ProtobufSourceDir/$ProtobufFilePattern").FullName | Resolve-Path -Relative
foreach ($protoFile in $protobufFiles) {
  Write-Output "Initializing for Protobuf File : $protoFile"
  $commandName = (Split-Path $protoFile -Leaf).Replace($ProtobufFileExtension, "")
  $commandIdLine = (Select-String -Path $protoFile -Pattern "CommandId=").Line
  $commandIdDelimiterIndex = $commandIdLine.IndexOf("=")
  $commandId = $commandIdLine.Substring($commandIdDelimiterIndex + 1)
  $javaCommandFileName = $commandName + $CommandClassFileSuffix + $JavaFileExtension
  $cppCommandHeaderFileName = $commandName + $CommandClassFileSuffix + $CppHeaderFileExtension
  $cppCommandClassFileName = $commandName + $CommandClassFileSuffix + $CppClassFileExtension

  Write-Output "Compiling Protobuf File : $protoFile"
  . $ProtocExe `
    --proto_path="$ProtobufSourceDir" `
    --cpp_out="lite:$CppOutputMessageDir" `
    --java_out="lite:$JavaRootDir" `
    "$protoFile"
  
  Write-Output "Generating Java Command File : $JavaOutputCommandDir/$javaCommandFileName"
  $javaCommandFile = $javaCommandClassTemplate.Replace($TemplateCommandNameField, $commandName)
  Out-File -Force -FilePath "$JavaOutputCommandDir/$javaCommandFileName" -InputObject $javaCommandFile -Encoding ASCII
  
  Write-Output "Adding Entry to Java Command Enum for : $commandName"
  $javaEnumCommandEntry = $javaCommandEnumEntryTemplate.Replace($TemplateCommandNameField, $commandName).Replace($TemplateCommandIdField, $commandId)
  $javaEnumEntryReplacement = "$javaEnumEntryReplacement`n" + `
                              "$javaEnumCommandEntry"
  
  Write-Output "Generating C++ Command Header File : $CppOutputCommandDir/$cppCommandHeaderFileName"
  $cppCommandHeaderFile = $cppCommandHeaderTemplate.Replace($TemplateCommandNameField, $commandName)
  Out-File -Force -FilePath "$CppOutputCommandDir/$cppCommandHeaderFileName" -InputObject $cppCommandHeaderFile -Encoding ASCII
  
  Write-Output "Generating C++ Command Class File : $CppOutputCommandDir/$cppCommandClassFileName"
  $cppCommandClassFile = $cppCommandClassTemplate.Replace($TemplateCommandNameField, $commandName).Replace($TemplateCommandEnumFileField, $CommandEnumFileName)
  Out-File -Force -FilePath "$CppOutputCommandDir/$cppCommandClassFileName" -InputObject $cppCommandClassFile -Encoding ASCII
  
  Write-Output "Adding Entry to C++ Command Enum for : $commandName"
  $cppEnumCommandEntry = $cppCommandEnumEntryTemplate.Replace($TemplateCommandNameField, $commandName).Replace($TemplateCommandIdField, $commandId)
  $cppEnumEntryReplacement = "$cppEnumEntryReplacement`n" +
                             "$cppEnumCommandEntry"
  
  Write-Output "Adding Entry to C++ Command Base TypeDef for : $commandName"
  $typeDefEntry = $cppCommandBaseTypeDefTemplate.Replace($TemplateCommandNameField, $commandName)
  $cppCommandBaseTypeDefReplacement = "$cppCommandBaseTypeDefReplacement`n" + `
                                      "`n" + `
                                      "$typeDefEntry"
}

Write-Output "Generating Java Command Enum File : $JavaCommandEnumFile"
$javaEnumEntryReplacement = $javaEnumEntryReplacement.Replace("`n$TemplateCommandEnumEntryField", "")
$javaCommandEnum = $javaCommandEnumTemplate.Replace($TemplateCommandEnumEntryField, $javaEnumEntryReplacement)
Out-File -Force -FilePath "$JavaCommandEnumFile" -InputObject $javaCommandEnum -Encoding ASCII

Write-Output "Generating C++ Command Enum File : $CppCommandEnumFile"
$cppEnumEntryReplacement = $cppEnumEntryReplacement.Replace("`n$TemplateCommandEnumEntryField", "")
$cppCommandEnum = $cppCommandEnumTemplate.Replace($TemplateCommandEnumEntryField, $cppEnumEntryReplacement)
Out-File -Force -FilePath "$CppCommandEnumFile" -InputObject $cppCommandEnum -Encoding ASCII

Write-Output "Generating C++ Command Base Header File : $CppCommandBaseHeaderFile"
$cppCommandBaseHeader = $cppCommandBaseHeaderTemplate.Replace($TemplateCommandNameField, $commandName)
Out-File -Force -FilePath "$CppCommandBaseHeaderFile" -InputObject $cppCommandBaseHeader -Encoding ASCII

Write-Output "Generating C++ Command Base Class File : $CppCommandBaseClassFile"
$cppCommandBaseTypeDefReplacement = $cppCommandBaseTypeDefReplacement.Replace("`n`n$TemplateCommandBaseTypeDefField", "")
$cppCommandBaseClass = $cppCommandBaseClassTemplate.Replace($TemplateCommandBaseTypeDefField, $cppCommandBaseTypeDefReplacement)
Out-File -Force -FilePath "$CppCommandBaseClassFile" -InputObject $cppCommandBaseClass -Encoding ASCII

Write-Output "Successfully Generated Protobuf Message & Command Files!"
