# File Names
$ModuleApkFileName = "MenrvaApp.apk"
$MenrvaLibFileName = "libMenrvaEngine.so"
$Fftw3LibFileName = "libfftw3.so"
$KissFftLibFileName = "libkissfft.so"
$SharedCppLibFileName = "libc++_shared.so"
$AudioEffectsConfigFileName = "audio_effects.xml"
$CommandEnumFileName = "MenrvaCommands"
$CommandClassFileSuffix = "_Command"
$CommandHandlerFileSuffix = "_Handler"
$ModulePatchScriptFileName = "patch_effects_config.sh"

# File Extensions
$ProtobufFileExtension = ".proto"
$CppHeaderFileExtension = ".h"
$CppClassFileExtension = ".cpp"
$JavaFileExtension = ".java"

# Directories
$RepoRootDir = ".."
$RootAppDir = "$RepoRootDir/menrva/app"
$RootSourceDir = "$RootAppDir/src/main"
$SourceTemplatesDir = "$RootSourceDir/templates"
$ProtobufSourceDir = "$RootSourceDir/protobuf"
$CppSourceDir = "$RootSourceDir/cpp"
$CppOutputCommandDir = "$CppSourceDir/commands"
$CppOutputMessageDir = "$CppOutputCommandDir/messages"
$CppOutputCommandHandlersDir = "$CppSourceDir/command_handlers"
$CppCommandHandlersDir = "$CppSourceDir/command_handlers"
$CppCommandHandlerMapDir = "$CppSourceDir/engine"
$JavaRootDir = "$RootSourceDir/java"
$JavaSourceDir = "$JavaRootDir/com/monkeystable/menrva"
$JavaOutputCommandDir = "$JavaSourceDir/commands"
$JavaOutputMessageDir = "$JavaOutputCommandDir/messages"
$ModuleFilesDir = "$RepoRootDir/module_files"
$ArtifactsRootDir = "$RepoRootDir/artifacts"
$BackendArtifactDir = "$ArtifactsRootDir/backend"
$MagiskModuleDir = "$RepoRootDir/libs/MMT-Ex"
$ModuleCommonDir = "$MagiskModuleDir/common"
$ModuleBackendDir = "$ModuleCommonDir/backend"
$VendorLibDir = "/vendor/lib"
$VendorEtcDir = "/vendor/etc"
$SoundFxLibDir = "/vendor/lib/soundfx"

# Full File Paths
$AdbExe = "$env:LOCALAPPDATA/Android/Sdk/platform-tools/adb.exe"
$ProtocExe = "$RepoRootDir/libs/protobuf/out/compiler/protoc.exe"
$CommandTemplateFile = "$SourceTemplatesDir/ProtobufCommand.proto.template"
$CppCommandEnumTemplateFile = "$SourceTemplatesDir/CommandsEnum.h.template"
$CppCommandEnumEntryTemplateFile = "$SourceTemplatesDir/CommandsEnum.h_Entry.template"
$CppCommandEnumFile = "$CppOutputCommandDir/$CommandEnumFileName$CppHeaderFileExtension"
$CppCommandBaseTypeDefTemplateFile = "$SourceTemplatesDir/TypedCommandBaseTypes.template"
$CppCommandBaseHeaderTemplateFile = "$SourceTemplatesDir/TypedCommandBase.h.template"
$CppCommandBaseClassTemplateFile = "$SourceTemplatesDir/TypedCommandBase.cpp.template"
$CppCommandBaseHeaderFile = "$CppOutputCommandDir/TypedCommandBase.h"
$CppCommandBaseClassFile = "$CppOutputCommandDir/TypedCommandBase.cpp"
$CppCommandHeaderTemplateFile = "$SourceTemplatesDir/CommandClass.h.template"
$CppCommandClassTemplateFile = "$SourceTemplatesDir/CommandClass.cpp.template"
$CppCommandHandlerHeaderTemplateFile = "$SourceTemplatesDir/CommandHandler.h.template"
$CppCommandHandlerClassTemplateFile = "$SourceTemplatesDir/CommandHandler.cpp.template"
$CppTypedCommandHandlerBaseHeaderTemplateFile = "$SourceTemplatesDir/TypedCommandHandlerBase.h.template"
$CppTypedCommandHandlerBaseClassTemplateFile = "$SourceTemplatesDir/TypedCommandHandlerBase.cpp.template"
$CppTypedCommandHandlerBaseTypeDefTemplateFile = "$SourceTemplatesDir/TypedCommandHandlerBaseTypes.template"
$CppTypedCommandHandlerBaseHeaderFile = "$CppOutputCommandHandlersDir/TypedCommandHandlerBase.h"
$CppTypedCommandHandlerBaseClassFile = "$CppOutputCommandHandlersDir/TypedCommandHandlerBase.cpp"
$CppHandlerMapHeaderTemplateFile = "$SourceTemplatesDir/CommandHandlerMap.h.template"
$CppHandlerMapClassTemplateFile = "$SourceTemplatesDir/CommandHandlerMap.cpp.template"
$CppHandlerMapIncludeTemplateFile = "$SourceTemplatesDir/CommandHandlerMapInclude.template"
$CppHandlerMapEntryTemplateFile = "$SourceTemplatesDir/CommandHandlerMapEntry.template"
$CppHandlerMapHeaderFile = "$CppCommandHandlerMapDir/CommandHandlerMap.h"
$CppHandlerMapClassFile = "$CppCommandHandlerMapDir/CommandHandlerMap.cpp"
$JavaCommandClassTemplateFile = "$SourceTemplatesDir/CommandClass.java.template"
$JavaCommandEnumTemplateFile = "$SourceTemplatesDir/CommandsEnum.java.template"
$JavaCommandEnumEntryTemplateFile = "$SourceTemplatesDir/CommandsEnum.java_Entry.template"
$JavaCommandEnumFile = "$JavaOutputCommandDir/$CommandEnumFileName$JavaFileExtension"
$AudioEffectsConfigFile = "$ModuleFilesDir/$AudioEffectsConfigFileName"
$ModuleCustomizeFile = "$ModuleFilesDir/customize.sh"
$ModuleInstallScriptFile = "$ModuleFilesDir/install.sh"
$ModulePatchScriptFile = "$ModuleFilesDir/$ModulePatchScriptFileName"
$ModuleInfoFile = "$ModuleFilesDir/module.prop"
$ModuleReadmeFile = "$RepoRootDir/Readme.md"
$MenrvaMagiskModuleFile = "$ArtifactsRootDir/Menrva_MagiskModule.zip"

# Template Fields
$TemplateCommandEnumFileField = "<CommandEnumFile>"
$TemplateCommandEnumEntryField = "<EnumEntry>"
$TemplateCommandIdField = "<CommandId>"
$TemplateCommandNameField = "<CommandName>"
$TemplateCommandBaseTypeDefField = "<CommandBaseTypeDef>"
$TemplateCommandHandlerBaseTypeDef = "<CommandHandlerBaseTypeDef>"
$TemplateHandlerMapIncludesField = "<HandlerMapIncludes>"
$TemplateHandlerMapEntriesField = "<HandlerMapEntries>"

# Patterns
$CommandHandlerExcludePattern = "Module_*"
$ProtobufFilePattern = "*$ProtobufFileExtension"
$CommandHandlerFilePattern = "*$CommandHandlerFileSuffix$CppClassFileExtension"
$BackendArtifactExcludePattern = "*Test*"
