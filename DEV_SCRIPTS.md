# Development Scripts Info
The Menrva Project provides various development scripts designed to automate some of the workflows associated with Menrva's development.  Details of the scripts' functionality, output and parameters are documented below.  See the [CONTRIBUTING](CONTRIBUTING.md) document for workflow details.

## Requirements
* Powershell
* 7zip

## [build_variables.ps1](scripts/build_variables.ps1)
A container for all of the constant values that are shared between the development scripts.  This provides a single place to update values and impact all of the development scripts.

### Execution Syntax
None; script is dot sourced into other development scripts

### Parameters
None

## [create_new_protobuf_command.ps1](scripts/create_new_protobuf_command.ps1)
Creates a new Protobuf Command File based on the [Protobuf Command Template File](menrva/app/src/main/templates/ProtobufCommand.proto.template) using the provided Command Name and places the resulting file in the Protobuf Commands directory for a developer to complete.

### Execution Syntax
./create_new_protobuf_command.ps1 [CommandName]

### Parameters
* CommandName - Name of the New Command

## [generate_protobuf_commands.ps1](scripts/generate_protobuf_commands.ps1)
Executes the Protobuf Compiler against all Protobuf Command Files to generate both Java & C++ source code files for the Protobuf Messages.  It also generates both Java & C++ Command source code files based on the Command Template Files ([Java Command Template](menrva/app/src/main/templates/CommandClass.java.template), [C++ Command Header Template](menrva/app/src/main/templates/CommandClass.h.template), [C++ Command Class Template](menrva/app/src/main/templates/CommandClass.cpp.template)) updates all necessary supporting Java & C++ source code files.

### Execution Syntax
./generate_protobuf_commands.ps1

### Parameters
None

## [create_new_command_handler.ps1](scripts/create_new_command_handler.ps1)
Creates a C++ Command Handler based on the C++ Command Handler Templates ([Command Handler Header Template](menrva/app/src/main/templates/CommandHandler.h.template) & [Command Handler Class Template](menrva/app/src/main/templates/CommandHandler.cpp.template)) using the provided Command Name and places the resulting file in the C++ Command Handlers directory for a developer to complete.  It also executes the [generate_command_handler_base.ps1](scripts/generate_command_handler_base.ps1) & [generate_command_handler_map.ps1](scripts/generate_command_handler_map.ps1) scripts in order to generate the necessary supporting code.

### Execution Syntax
./create_new_command_handler.ps1 [CommandName]

### Parameters
* CommandName - Name of the Command to Handle

## [generate_command_handler_base.ps1](scripts/generate_command_handler_base.ps1)
Regenerates the C++ TypedCommandHandlerBase Header & Class by incorporating strong typedefs for all Command Handlers found in [menrva/app/src/main/cpp/command_handlers](menrva/app/src/main/cpp/command_handlers).  This script is executed as part of the [create_new_command_handler.ps1](scripts/create_new_command_handler.ps1) workflow.

### Execution Syntax
./generate_command_handler_base.ps1

### Parameters
None

## [generate_command_handler_map.ps1](scripts/generate_command_handler_map.ps1)
Regenerates the C++ CommandHandlerMap Header & Class by populating the HandlerMap with appropriate entries for each Command Handler and its associated Command.  This script is executed as part of the [create_new_command_handler.ps1](scripts/create_new_command_handler.ps1) workflow.

### Execution Syntax
./generate_command_handler_map.ps1

### Parameters
None

## [build.ps1](scripts/build.ps1)
Executes the gradle build process for the Project with the appropriate Product Flavor and Build Type parameters.

### Execution Syntax
./build.ps1 [IsOptimizedBuild] [IsReleaseBuild]

### Parameters
* IsOptimizedBuild - (Optional; Default 'true') Indicates whether to build the Project with all optimizations
* IsReleaseBuild - (Optional; Default 'true') Indicates whether to build the Project as Release or Debug

## [prepare_artifacts.ps1](scripts/prepare_artifacts.ps1)
Copies a successful build's APK to the Artifacts directory and extracts the backend libraries by architecture to prepare for deployment.  This script requires a successful build to be performed prior to execution.

### Execution Syntax
./prepare_artifacts.ps1 [IsOptimizedBuild] [IsReleaseBuild] [IsSigned]

### Parameters
* IsOptimizedBuild - (Optional; Default true) Indicates whether to use Optimized Binaries
* IsReleaseBuild - (Optional; Default true) Indicates whether to use Release Binaries
* IsSigned - (Optional; Default false) Indicates whether the Binaries are signed

## [deploy_to_device.ps1](scripts/deploy_to_device.ps1)
Attempts to deploy the specified Build Artifacts to an attached device via ADB.  This script requires successful execution of [prepare_artifacts.ps1](scripts/prepare_artifacts.ps1) to be performed prior to execution.

### Execution Syntax
./deploy_to_device.ps1 [Architecture] [BuildType] [EffectsConfigFile]

### Parameters
* Architecture - (Optional; Default 'x86') Indicates the Architecture of the Target Device
* BuildType - (Optional; Default 'debug') Indicates the type of successful build to leverage for deployment
* EffectsConfigFile - (Optional; Default '/vendor/etc/audio_effects.xml') Provides the path to the audio_effects config file on the target device; usually either '/vendor/etc/audio_effects.xml' or '/vendor/etc/audio_effects.conf'

## [prepare_magisk_module_template.ps1](scripts/prepare_magisk_module_template.ps1)
Downloads and extracts the MMT-Extended GitHub Repository to libs/MMT-Extended-master/.

### Execution Syntax
./prepare_magisk_module_template.ps1

### Parameters
N/A

## [generate_magisk_module.ps1](scripts/generate_magisk_module.ps1)
Regenerates a Magisk Module using the [MMT-Extended Template](https://github.com/Zackptg5/MMT-Extended) and places the Magisk Zip File in the Artifacts directory.  This script requires successful execution of [prepare_artifacts.ps1](scripts/prepare_artifacts.ps1) and [prepare_magisk_module_template.ps1](scripts/prepare_magisk_module_template.ps1) to be performed.

### Execution Syntax
./generate_magisk_module.ps1

### Parameters
N/A
