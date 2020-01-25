param([String]$commandName)
if ([String]::IsNullOrWhiteSpace($commandName)) {
    Write-Output "Must supply a Command Name as script argument."
    Write-Output "Example : ./create_new_protobuf_command.ps1 CommandName"
    exit 1
}

#$CommandTemplateFile = "./menrva/app/src/main/templates/ProtobufCommand.proto.template"
#$ProtobufSourceDir = "./menrva/app/src/main/protobuf"
#$ProtobufFileExtension = ".proto"
#$CommandFileName = "$commandName$ProtobufFileExtension"
#$CommandFilePath = "$ProtobufSourceDir/$CommandFileName"

#$TemplateCommandIdField = "<CommandId>"
#$TemplateCommandNameField = "<CommandName>"

. ./build_variables.ps1
$CommandFileName = "$commandName$ProtobufFileExtension"
$CommandFilePath = "$ProtobufSourceDir/$CommandFileName"

If (Test-Path "$CommandFilePath") {
    Write-Output "Protobuf Command File ($CommandFilePath) already exists!"
    exit 1
}

Write-Output "Getting Protobuf Command Template..."
$commandTemplate = Get-Content -Path $CommandTemplateFile

Write-Output "Calculating New Command Id..."
$commandId = (Get-ChildItem -File -Path $ProtobufSourceDir).Count

Write-Output "Generating Protobuf Command for : $commandName"
$commandOutput = $commandTemplate.Replace($TemplateCommandIdField, $commandId).Replace($TemplateCommandNameField, $commandName)

Write-Output "Creating Protobuf Command File : $commandFileName"
Out-File -Force -FilePath "$CommandFilePath" -InputObject $commandOutput -Encoding ASCII
