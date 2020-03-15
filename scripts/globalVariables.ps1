# Misc
$CurrentYear = Get-Date -Format "yyyy"

# File Extensions
$ProtobufFileExtension = ".proto"

# File Names
$CommandClassFileSuffix = "_Command"
$CommandEnumFileName = "MenrvaCommands"

# Executable Paths
$ProtocExe = "$RepoRootDir/libs/protobuf/out/compiler/protoc.exe"

# Template Fields
$TemplateYearField = "<Year>"
$TemplateCommandNameField = "<CommandName>"
$TemplateCommandIdField = "<CommandId>"
$TemplateCommandEnumEntryField = "<EnumEntry>"

# Patterns
$ProtobufFilePattern = "*$ProtobufFileExtension"
$CommandIdDelimiter = "="
$CommandIdPrefixPattern = "CommandId$CommandIdDelimiter"
