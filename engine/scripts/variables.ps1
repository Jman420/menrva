. ../../scripts/globalVariables.ps1

# File Names
$CommandHandlerFileSuffix = "_Handler"

# File Extensions
$ProtobufFileExtension = ".proto"
$CppHeaderFileExtension = ".h"
$CppClassFileExtension = ".cpp"

# Directories
$RepoRootDir = "../.."
$EngineRootDir = "$RepoRootDir/engine"
$RootSourceDir = "$EngineRootDir/src"
$ProtobufSourceDir = "$EngineRootDir/protobuf"
$SourceTemplatesDir = "$EngineRootDir/templates"
$CppCommandSourceDir = "$RootSourceDir/commands"
$CppMessageSourceDir = "$CppCommandSourceDir/messages"
$CppCommandHandlersSourceDir = "$RootSourceDir/command_handlers"

# Executable Paths
$ProtocExe = "$RepoRootDir/libs/protobuf/out/compiler/protoc.exe"

# Template File Paths
$CommandTemplateFile = "$SourceTemplatesDir/ProtobufCommand.proto.template"
$CppCommandHeaderTemplateFile = "$SourceTemplatesDir/CommandClass.h.template"
$CppCommandClassTemplateFile = "$SourceTemplatesDir/CommandClass.cpp.template"
$CppCommandEnumTemplateFile = "$SourceTemplatesDir/CommandsEnum.h.template"
$CppCommandEnumEntryTemplateFile = "$SourceTemplatesDir/CommandsEnum.h_Entry.template"
$CppCommandBaseHeaderTemplateFile = "$SourceTemplatesDir/TypedCommandBase.h.template"
$CppCommandBaseClassTemplateFile = "$SourceTemplatesDir/TypedCommandBase.cpp.template"
$CppCommandBaseTypeDefTemplateFile = "$SourceTemplatesDir/TypedCommandBaseTypes.template"
$CppTypedCommandHandlerBaseHeaderTemplateFile = "$SourceTemplatesDir/TypedCommandHandlerBase.h.template"
$CppTypedCommandHandlerBaseClassTemplateFile = "$SourceTemplatesDir/TypedCommandHandlerBase.cpp.template"
$CppTypedCommandHandlerBaseTypeDefTemplateFile = "$SourceTemplatesDir/TypedCommandHandlerBaseTypes.template"
$CppCommandHandlerMapHeaderTemplateFile = "$SourceTemplatesDir/CommandHandlerMap.h.template"
$CppCommandHandlerMapClassTemplateFile = "$SourceTemplatesDir/CommandHandlerMap.cpp.template"
$CppCommandHandlerMapIncludeTemplateFile = "$SourceTemplatesDir/CommandHandlerMapInclude.template"
$CppCommandHandlerMapEntryTemplateFile = "$SourceTemplatesDir/CommandHandlerMapEntry.template"

# Source Code File Paths
$CppCommandEnumFile = "$CppOutputCommandDir/$CommandEnumFileName$CppHeaderFileExtension"
$CppCommandBaseHeaderFile = "$CppOutputCommandDir/TypedCommandBase$CppHeaderFileExtension"
$CppCommandBaseClassFile = "$CppOutputCommandDir/TypedCommandBase$CppClassFileExtension"
$CppTypedCommandHandlerBaseHeaderFile = "$CppOutputCommandHandlersDir/TypedCommandHandlerBase$CppHeaderFileExtension"
$CppTypedCommandHandlerBaseClassFile = "$CppOutputCommandHandlersDir/TypedCommandHandlerBase$CppClassFileExtension"
$CppCommandHandlerMapHeaderFile = "$CppCommandHandlerMapDir/CommandHandlerMap$CppHeaderFileExtension"
$CppCommandHandlerMapClassFile = "$CppCommandHandlerMapDir/CommandHandlerMap$CppClassFileExtension"

# Template Fields
$TemplateYearField = "<Year>"
$TemplateCommandIdField = "<CommandId>"
$TemplateCommandNameField = "<CommandName>"
$TemplateCommandEnumFileField = "<CommandEnumFile>"
$TemplateCommandEnumEntryField = "<EnumEntry>"
$TemplateCommandBaseTypeDefField = "<CommandBaseTypeDef>"
$TemplateCommandHandlerBaseTypeDef = "<CommandHandlerBaseTypeDef>"
$TemplateHandlerMapIncludesField = "<HandlerMapIncludes>"
$TemplateHandlerMapEntriesField = "<HandlerMapEntries>"

# Patterns
$ProtobufFilePattern = "*$ProtobufFileExtension"
$CommandHandlerFilePattern = "*$CommandHandlerFileSuffix$CppClassFileExtension"
