param([String]$commandName)

$CommandTemplateFile = "./menrva/app/src/main/templates/ProtobufCommand.proto.template"
$CommandOutputDir = "./menrva/app/src/main/protobuf"
$ProtobufFileExtension = ".proto"
$CommandFileName = "$commandName$ProtobufFileExtension"
$CommandFilePath = "$CommandOutputDir/$CommandFileName"

$TemplateCommandIdField = "<CommandId>"
$TemplateCommandNameField = "<CommandName>"

If (Test-Path "$CommandFilePath") {
    Write-Output "Protobuf Command File ($CommandFilePath) already exists!"
    exit 1
}

Write-Output "Getting Protobuf Command Template..."
$commandTemplate = Get-Content -Path $CommandTemplateFile

Write-Output "Calculating New Command Id..."
$commandId = (Get-ChildItem -File -Path $CommandOutputDir).Count

Write-Output "Generating Protobuf Command for : $commandName"
$commandOutput = $commandTemplate.Replace($TemplateCommandIdField, $commandId).Replace($TemplateCommandNameField, $commandName)

Write-Output "Creating Protobuf Command File : $commandFileName"
Out-File -Force -FilePath "$CommandFilePath" -InputObject $commandOutput -Encoding ASCII