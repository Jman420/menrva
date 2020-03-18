$RepoRootDir = "../../.."

. ../../../scripts/globalVariables.ps1

# File Extensions
$JavaFileExtension = ".java"

# File Names
$MagiskModuleApkFileName = "MenrvaApp.apk"
$MenrvaLibFileName = "libMenrvaEngine-Android.so"
$SharedCppLibFileName = "libc++_shared.so"
$AudioEffectsConfigFileName = "audio_effects.xml"

# Directories
$AndroidHostDir = "$RepoRootDir/hosts/android"
$AndroidProjectDir = "$AndroidHostDir/src"
$AndroidAppDir = "$AndroidProjectDir/app"
$ArtifactsRootDir = "$AndroidHostDir/artifacts"
$BackendArtifactDir = "$ArtifactsRootDir/lib"
$AndroidLibsDir = "$AndroidHostDir/libs"
$MagiskModuleDir = "$AndroidLibsDir/MMT-Extended"
$MagiskModuleCommonDir = "$MagiskModuleDir/common"
$MagiskModuleFilesDir = "$AndroidHostDir/module_files"
$VendorLibDir = "/vendor/lib"
$VendorEtcDir = "/vendor/etc"
$SoundFxLibDir = "/vendor/lib/soundfx"
$JavaSourceDir = "$AndroidAppDir/src/main/java"
$JavaSourceRootDir = "$JavaSourceDir/com/monkeystable/menrva"
$JavaCommandOutputDir = "$JavaSourceRootDir/commands"
$JavaMessageOutputDir = "$JavaCommandOutputDir/messages"
$SourceTemplatesDir = "$AndroidHostDir/templates"
$ProtobufSourceDir = "$RepoRootDir/engine/protobuf"

# Full File Paths
$AdbExe = "$env:LOCALAPPDATA/Android/Sdk/platform-tools/adb.exe"
$ReadmeFile = "$RepoRootDir/Readme.md"
$MenrvaMagiskModuleFile = "$ArtifactsRootDir/Menrva_MagiskModule.zip"

# Source Code File Paths
$JavaCommandClassTemplateFile = "$SourceTemplatesDir/CommandClass.java.template"
$JavaCommandEnumTemplateFile = "$SourceTemplatesDir/CommandsEnum.java.template"
$JavaCommandEnumEntryTemplateFile = "$SourceTemplatesDir/CommandsEnum.java_Entry.template"
$JavaCommandEnumFile = "$JavaCommandOutputDir/$CommandEnumFileName$JavaFileExtension"
