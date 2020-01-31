param([String]$CommandName)
if ([String]::IsNullOrWhiteSpace($CommandName)) {
    Write-Output "Must supply a Command Name as script argument."
    Write-Output "Example : ./create_new_protobuf_command.ps1 [CommandName]"
    exit 1
}

. ./variables.ps1
$CommandFileName = "$CommandName$ProtobufFileExtension"
$CommandFilePath = "$ProtobufSourceDir/$CommandFileName"

If (Test-Path "$CommandFilePath") {
    Write-Output "Protobuf Command File ($CommandFilePath) already exists!"
    exit 1
}

Write-Output "Getting Protobuf Command Template..."
$commandTemplate = Get-Content -Path $CommandTemplateFile

Write-Output "Calculating New Command Id..."
$commandId = (Get-ChildItem -File -Path $ProtobufSourceDir).Count

Write-Output "Generating Protobuf Command for : $CommandName"
$commandOutput = $commandTemplate.Replace($TemplateCommandIdField, $commandId).Replace($TemplateCommandNameField, $CommandName)

Write-Output "Creating Protobuf Command File : $commandFileName"
Out-File -Force -FilePath "$CommandFilePath" -InputObject $commandOutput -Encoding ASCII

Write-Output "Successfully created Protobuf Command File : $CommandFilePath !"
