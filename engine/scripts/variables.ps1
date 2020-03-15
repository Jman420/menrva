. ../../scripts/globalVariables.ps1

# Misc
$CurrentYear = Get-Date -Format "yyyy"

# File Names
$CommandHandlerFileSuffix = "_Handler"
$CommandEnumFileName = "MenrvaCommands"

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
$CppCommandHandlerMapSourceDir = "$RootSourceDir/engine"

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
$CppCommandHandlerHeaderTemplateFile = "$SourceTemplatesDir/CommandHandler.h.template"
$CppCommandHandlerClassTemplateFile = "$SourceTemplatesDir/CommandHandler.cpp.template"
$CppCommandHandlerMapHeaderTemplateFile = "$SourceTemplatesDir/CommandHandlerMap.h.template"
$CppCommandHandlerMapClassTemplateFile = "$SourceTemplatesDir/CommandHandlerMap.cpp.template"
$CppCommandHandlerMapIncludeTemplateFile = "$SourceTemplatesDir/CommandHandlerMapInclude.template"
$CppCommandHandlerMapEntryTemplateFile = "$SourceTemplatesDir/CommandHandlerMapEntry.template"

# Source Code File Paths
$CppCommandEnumFile = "$CppCommandSourceDir/$CommandEnumFileName$CppHeaderFileExtension"
$CppCommandBaseHeaderFile = "$CppCommandSourceDir/TypedCommandBase$CppHeaderFileExtension"
$CppCommandBaseClassFile = "$CppCommandSourceDir/TypedCommandBase$CppClassFileExtension"
$CppTypedCommandHandlerBaseHeaderFile = "$CppCommandHandlersSourceDir/TypedCommandHandlerBase$CppHeaderFileExtension"
$CppTypedCommandHandlerBaseClassFile = "$CppCommandHandlersSourceDir/TypedCommandHandlerBase$CppClassFileExtension"
$CppCommandHandlerMapHeaderFile = "$CppCommandHandlerMapSourceDir/CommandHandlerMap$CppHeaderFileExtension"
$CppCommandHandlerMapClassFile = "$CppCommandHandlerMapSourceDir/CommandHandlerMap$CppClassFileExtension"

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
