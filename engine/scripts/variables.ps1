$RepoRootDir = "../.."

. ../../scripts/globalVariables.ps1

# File Extensions
$CppHeaderFileExtension = ".h"
$CppClassFileExtension = ".cpp"

# File Names
$CommandHandlerFileSuffix = "_Handler"

# Directories
$EngineRootDir = "$RepoRootDir/engine"
$RootSourceDir = "$EngineRootDir/src"
$ProtobufSourceDir = "$EngineRootDir/protobuf"
$SourceTemplatesDir = "$EngineRootDir/templates"
$CppCommandSourceDir = "$RootSourceDir/commands"
$CppMessageSourceDir = "$CppCommandSourceDir/messages"
$CppCommandHandlersSourceDir = "$RootSourceDir/command_handlers"
$CppCommandHandlerMapSourceDir = "$RootSourceDir/engine"

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
$TemplateCommandEnumFileField = "<CommandEnumFile>"
$TemplateCommandBaseTypeDefField = "<CommandBaseTypeDef>"
$TemplateCommandHandlerBaseTypeDef = "<CommandHandlerBaseTypeDef>"
$TemplateHandlerMapIncludesField = "<HandlerMapIncludes>"
$TemplateHandlerMapEntriesField = "<HandlerMapEntries>"

# Patterns
$CommandHandlerFilePattern = "*$CommandHandlerFileSuffix$CppClassFileExtension"
