# Menrva Engine Build Instructions

## Development Requirements
  - C++ IDE with CMake Support (recommendation : Visual Studio w/ Clang Support)
  - Powershell

## Dependencies
  - [KFRLib](https://www.kfrlib.com/)
  - [Protobuf](https://developers.google.com/protocol-buffers)

## Build Dependencies
See [CONTRIBUTING](../../../docs/CONTRIBUTING.md#building-dependencies) for details on building Project Dependencies.

## Generating Templated Engine Source Code
MenrvaEngine uses Templated Source Code for some portions of it's codebase for portability and simplifying certain development processes.  Scripts have also been provided to automate using and populating the Template Source Code files when necessary.  Source Code Files which are generated from Templates will carry a header comment identifying it as generated source code and provide the path to the Source Code Template file.  These files are not tracked by source control and are explicitly ignored in the root gitignore.

### Steps
  - Use [/engine/scripts/generate_protobuf_commands.ps1](../scripts/generate_protobuf_commands.ps1) to generate the necessary C++ Source Code from the Protobuf Command Files
  - Use [/engine/scripts/generate_command_handler_base.ps1](../scripts/generate_command_handler_base.ps1) to generate the TypedCommandHandlerBase Class Source Files
  - Use [/engine/scripts/generate_command_handler_map.ps1](../scripts/generate_command_handler_map.ps1) to generate the CommandHandlerMap Class Source Files

## Building the Engine
Once the above steps in [Generating Templated Engine Source Code](#generating-templated-engine-source-code) have been completed the MenrvaEngine Project is ready to be built.  There are many ways that the CMake Project can be compiled and won't be covered here.  Visual Studio is recommended using the CMake Project support; when using Visual Studio the Project can be built as any other Visual Studio Project.

After any successful Engine Build the [/engine/scripts/copy_include_headers.ps1](../scripts/copy_include_headers.ps1) script should be executed to ensure that the Include Headers are kept up to date with the Binaries.  This is required for integrating MenrvaEngine into any other Projects including Host Implementations.

## Adding New Engine Commands
MenrvaEngine uses a Command-based API powered by [Protobuf](https://developers.google.com/protocol-buffers).  This allows for easily adding new features and functionality to the Engine with a decoupled mechanism for inter-process communications.  Scripts and templates are provided to simplify the processes of creating new Protobuf Command Files,  generating source code files from Protobuf Command Files and generating new command handler source code.

### Steps
  - Use [/engine/scripts/create_new_protobuf_command.ps1](../scripts/create_new_protobuf_command.ps1) to create a new Protobuf Command File
  - Edit the newly created Protobuf Command File's Request & Response structures with any necessary data points
  - Use [/engine/scripts/generate_protobuf_commands.ps1](../scripts/generate_protobuf_commands.ps1) to generate the necessary C++ Source Code from the Protobuf Command Files
  - Use [/engine/scripts/create_new_command_handler.ps1](../scripts/create_new_command_handler.ps1) to generate a new Command Handler for the newly created Command
  - Add logic to the newly created Command Handler's Source Code File in [/engine/src/command_handlers](../src/command_handlers)

## Configuring Calculation Precision
See [Calculation Precision](../README.md#calculation-precision) for details about Calculation Precision.

### Enabling Double Precision
  - Uncomment the ```//#define MENRVA_DOUBLE_PRECISION``` line in [/engine/src/BuildConfig.h](../src/BuildConfig.h)
