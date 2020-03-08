# Engine Architecture Details

## Objects
- [Host Interface](#Host-Interface)
- [Module Context](#Module-Context)
- [Command Processor](#Command-Processor)
- [Effects Engine](#Effects-Engine)
- [Effects Bundles](#Effects-Bundles)

## Host Interface
Provides the necessary Public Interface for integration into a Host Environment such as Android, iOS, Windows, Mac, etc.

Implementations : [AndroidInterface](../menrva/app/src/main/cpp/hosts/android/AndroidInterface.h)

## Module Context
Provides pointers to the necessary Objects for an instance of Menrva to function.  The ModuleContext referenced below can be extended in order to add Host Specific details.

Implementations : [ModuleContext](../menrva/app/src/main/cpp/hosts/ModuleStructures.h), [AndroidModuleContext](../menrva/app/src/main/cpp/hosts/android/AndroidStructures.h)

## Command Processor
Provides a Command Based Interface for interacting with a Menrva Instance.  Host Specific Command Handlers can be injected into the Command Processor by adding them to the HandlerMap contained in the CommandHandlerMap (example can be found in [AndroidInterface](../menrva/app/src/main/cpp/hosts/android/AndroidInterface.h)).

Implementation : [CommandProcessor](../menrva/app/src/main/cpp/engine/CommandProcessor.h)

## Effects Engine
Provides the actual DSP Effects Engine functionality.  Contains instances of Single- & Multi-Channel Effects Bundles and manages the Audio Processing Pipeline. 

Implementation : [EffectsEngine](../menrva/app/src/main/cpp/engine/EffectsEngine.h)

## Effects Bundles
TODO : Describe purpose & how to add new effects

Implementations : [SingleChannelEffectsBundle](), [MultiChannelEffectsBundle]()
