$ProtocExe = "./libs/protobuf/out/compiler/protoc.exe"
$ProtobufFileExtension = ".proto"
$ProtobufFilePattern = "*$ProtobufFileExtension"

$RootSourceDir = "./menrva/app/src/main"
$ProtobufSourceDir = "$RootSourceDir/protobuf"
$SourceTemplatesDir = "$RootSourceDir/templates"
$CppOutputCommandDir = "$RootSourceDir/cpp/commands"
$CppOutputMessageDir = "$CppOutputCommandDir/messages"
$JavaSourceDir = "$RootSourceDir/java"
$JavaOutputCommandDir = "$JavaSourceDir/com/monkeystable/menrva/commands"
$JavaOutputMessageDir = "$JavaOutputCommandDir/messages"

$TemplateCommandBaseTypeDefField = "<CommandBaseTypeDef>"
$TemplateCommandNameField = "<CommandName>"
$TemplateCommandEnumFileField = "<CommandEnumFile>"
$CommandEnumFileName = "MenrvaCommands"
$CommandClassFileSuffix = "_Command"

$JavaFileExtension = ".java"
$JavaCommandClassTemplateFile = "$SourceTemplatesDir/CommandClass.java.template"
$JavaCommandEnumTemplateFile = "$SourceTemplatesDir/CommandsEnum.java.template"
$JavaCommandEnumFile = "$JavaOutputCommandDir/$CommandEnumFileName$JavaFileExtension"

$CppHeaderFileExtension = ".h"
$CppClassFileExtension = ".cpp"
$CppCommandHeaderTemplateFile = "$SourceTemplatesDir/CommandClass.h.template"
$CppCommandClassTemplateFile = "$SourceTemplatesDir/CommandClass.cpp.template"
$CppCommandEnumTemplateFile = "$SourceTemplatesDir/CommandsEnum.h.template"
$CppCommandEnumFile = "$CppOutputCommandDir/$CommandEnumFileName$CppHeaderFileExtension"
$CppCommandBaseTypeDefTemplateFile = "$SourceTemplatesDir/menrva_command_base_types.template"
$CppCommandBaseHeaderTemplateFile = "$SourceTemplatesDir/menrva_command_base.h.template"
$CppCommandBaseClassTemplateFile = "$SourceTemplatesDir/menrva_command_base.cpp.template"
$CppCommandBaseHeaderFile = "$CppOutputCommandDir/menrva_command_base.h"
$CppCommandBaseClassFile = "$CppOutputCommandDir/menrva_command_base.cpp"

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

$cppCommandHeaderTemplate = Get-Content -Path $CppCommandHeaderTemplateFile
$cppCommandClassTemplate = Get-Content -Path $CppCommandClassTemplateFile
$cppCommandEnumTemplate = Get-Content -Path $CppCommandEnumTemplateFile
$cppCommandBaseTypeDefTemplate = (Get-Content -Path $CppCommandBaseTypeDefTemplateFile) -Join "`n"
$cppCommandBaseHeaderTemplate = Get-Content -Path $CppCommandBaseHeaderTemplateFile
$cppCommandBaseClassTemplate = Get-Content -Path $CppCommandBaseClassTemplateFile

Write-Output "Compiling Protobuf Message & Command Files..."
$javaCommandEnum = $javaCommandEnumTemplate
$cppCommandEnum = $cppCommandEnumTemplate
$cppCommandBaseTypeDef = $TemplateCommandBaseTypeDefField
$protobufFiles = (Get-ChildItem -Path "$ProtobufSourceDir/$ProtobufFilePattern" -Recurse).FullName | Resolve-Path -Relative
foreach ($protoFile in $protobufFiles) {
  Write-Output "Initializing for Protobuf File : $protoFile"
  $commandName = (Split-Path $protoFile -Leaf).Replace($ProtobufFileExtension, "")
  $javaCommandFileName = $commandName + $CommandClassFileSuffix + $JavaFileExtension
  $cppCommandHeaderFileName = $commandName + $CommandClassFileSuffix + $CppHeaderFileExtension
  $cppCommandClassFileName = $commandName + $CommandClassFileSuffix + $CppClassFileExtension

  Write-Output "Compiling Protobuf File : $protoFile"
  . $ProtocExe `
    --proto_path="$ProtobufSourceDir" `
    --cpp_out="lite:$CppOutputMessageDir" `
    --java_out="lite:$JavaSourceDir" `
    "$protoFile"
  
  Write-Output "Generating Java Command File : $JavaOutputCommandDir/$javaCommandFileName"
  $javaCommandFile = $javaCommandClassTemplate.Replace($TemplateCommandNameField, $commandName)
  Out-File -Force -FilePath "$JavaOutputCommandDir/$javaCommandFileName" -InputObject $javaCommandFile -Encoding ASCII
  
  Write-Output "Adding Entry to Java Command Enum for : $commandName"
  $enumCommandNameReplacement = @"
$commandName,
    $TemplateCommandNameField
"@
  $javaCommandEnum = $javaCommandEnum.Replace($TemplateCommandNameField, $enumCommandNameReplacement)
  
  Write-Output "Generating C++ Command Header File : $CppOutputCommandDir/$cppCommandHeaderFileName"
  $cppCommandHeaderFile = $cppCommandHeaderTemplate.Replace($TemplateCommandNameField, $commandName)
  Out-File -Force -FilePath "$CppOutputCommandDir/$cppCommandHeaderFileName" -InputObject $cppCommandHeaderFile -Encoding ASCII
  
  Write-Output "Generating C++ Command Class File : $CppOutputCommandDir/$cppCommandClassFileName"
  $cppCommandClassFile = $cppCommandClassTemplate.Replace($TemplateCommandNameField, $commandName).Replace($TemplateCommandEnumFileField, $CommandEnumFileName)
  Out-File -Force -FilePath "$CppOutputCommandDir/$cppCommandClassFileName" -InputObject $cppCommandClassFile -Encoding ASCII
  
  Write-Output "Adding Entry to C++ Command Enum for : $commandName"
  $cppCommandEnum = $cppCommandEnum.Replace($TemplateCommandNameField, $enumCommandNameReplacement)
  
  Write-Output "Adding Entry to C++ Command Base TypeDef for : $commandName"
  $typeDefEntry = $cppCommandBaseTypeDefTemplate.Replace($TemplateCommandNameField, $commandName)
  $cppCommandBaseTypeDefReplacement = @"
$typeDefEntry

$TemplateCommandBaseTypeDefField
"@
  $cppCommandBaseTypeDef = $cppCommandBaseTypeDef.Replace($TemplateCommandBaseTypeDefField, $cppCommandBaseTypeDefReplacement)
}

Write-Output "Generating Java Command Enum File : $JavaCommandEnumFile"
$javaCommandEnum = (($javaCommandEnum -Split "`n") | ? {$_ -NotMatch "$TemplateCommandNameField"}) -Join "`n"
Out-File -Force -FilePath "$JavaCommandEnumFile" -InputObject $javaCommandEnum -Encoding ASCII

Write-Output "Generating C++ Command Enum File : $CppCommandEnumFile"
$cppCommandEnum = (($cppCommandEnum -Split "`n") | ? {$_ -NotMatch "$TemplateCommandNameField"}) -Join "`n"
Out-File -Force -FilePath "$CppCommandEnumFile" -InputObject $cppCommandEnum -Encoding ASCII

Write-Output "Generating C++ Command Base Header File : $CppCommandBaseHeaderFile"
$cppCommandBaseHeader = $cppCommandBaseHeaderTemplate.Replace($TemplateCommandNameField, $commandName)
Out-File -Force -FilePath "$CppCommandBaseHeaderFile" -InputObject $cppCommandBaseHeader -Encoding ASCII

Write-Output "Generating C++ Command Base Class File : $CppCommandBaseClassFile"
$cppCommandBaseTypeDef = $cppCommandBaseTypeDef.Replace("`n$TemplateCommandBaseTypeDefField", "")
$cppCommandBaseClass = $cppCommandBaseClassTemplate.Replace($TemplateCommandBaseTypeDefField, $cppCommandBaseTypeDef)
Out-File -Force -FilePath "$CppCommandBaseClassFile" -InputObject $cppCommandBaseClass -Encoding ASCII

Write-Output "Successfully Generated Protobuf Message & Command Files!"
