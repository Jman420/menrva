$ProtocExe = "./libs/protobuf/out/compiler/protoc.exe"
$ProtobufSourceDir = "./menrva/app/src/main/protobuf"
$CppOutputDir = "./menrva/app/src/main/cpp/commands/messages"
$JavaOutputDir = "./menrva/app/src/main/java"
$FullJavaOutputDir = "$JavaOutputDir/com/monkeystable/menrva/commands/messages/"

$ProtobufFilePattern = "*.proto"

# Delete all files in Output Directories
if (Test-Path $CppOutputDir) {
    Write-Output "Removing C++ Output Directory : $CppOutputDir ..."
    Remove-Item $CppOutputDir -Force -Recurse
}
Write-Output "Creating C++ Output Directory : $CppOutputDir ..."
New-Item -ItemType directory -Force -Path $CppOutputDir

if (Test-Path $FullJavaOutputDir) {
    Write-Output "Removing Java Output Directory : $FullJavaOutputDir ..."
    Remove-Item $FullJavaOutputDir -Force -Recurse
}
Write-Output "Creating Java Output Directory : $FullJavaOutputDir ..."
New-Item -ItemType directory -Force -Path $FullJavaOutputDir

# Find all proto files in protobuf directory
Write-Output "Compiling Protobuf Files to C++ & Java..."
$protobufFiles = (Get-ChildItem -Path $ProtobufSourceDir/$ProtobufFilePattern -Recurse).FullName | Resolve-Path -Relative
foreach ($protoFile in $protobufFiles) {
  Write-Output "Compiling file : $protoFile"
  . $ProtocExe `
    --proto_path="$ProtobufSourceDir" `
    --cpp_out="lite:$CppOutputDir" `
    --java_out="lite:$JavaOutputDir" `
    "$protoFile"  
}
Write-Output "Successfully compiled Protobuf Files!"
