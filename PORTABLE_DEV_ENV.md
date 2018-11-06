# Installing OS to a USB Drive

## Requirements
  - VirtualBox
  - OS Installer ISO
  - DiskGenius
  - USB Drive or External Drive (82GB minimum for OS; 132GB recommended for daily driver equivalent)

## Creating EFI/ESP Partition
  - IMPORTANT : EFI Partition must be within the first 2.2TB of the disk
  - IMPORTANT : EFI Partition must be FAT32 File System
  - RECOMMENDED : EFI Partition should be the first partition 
  - RECOMMENDED : EFI Partition should be between 100-500MB
  - Run DiskGenius
  - Click on the USB Drive from the list on the left
  - Right click in the 'Partitions' list
  - Select 'Create ESP/MSR Partition'
  - Adjust the size of the ESP Partition if needed
  - Uncheck the 'Create MSR partition' checkbox
  - Click OK Button
  - Click 'Save All' Button at top right of Window
  - Click Yes to apply changes
  - Right click the 'ESP(0)' entry in the Partitions List
  - Click 'Format Current Partition'
  - Set File System to 'FAT32'
  - Set Volume Label to 'ESP'
  - Uncheck all checkboxes
  - Click 'Format' Button

## Setting up the Installer VM
  - Run VirtualBox
  - Create a New VM for the appropriate OS (this is our Installer VM)
  - Setup the System Settings as usual
  - IMPORTANT : On 'Hard disk' select 'Do not add a virtual hard disk'
  - Open Settings for the Installer VM
  - IMPORTANT : Under 'System Settings' check 'Enable EFI'
  - Update any System Settings necessary
  - Under Storage set the OS Installer Disk ISO as the CD Drive
  - Under Network setup a Bridged Adapter
  - Under USB select the appropriate USB Controller & Add the USB Device to the Filters
  - Click OK Button
  
## Installing the OS
  - Run VirtualBox
  - Start the Installer VM
  - Launch the OS Installer
  - Follow the Installer Wizard
  - On 'Updates and other software' check 'Install third-party software for ...'
  - IMPORTANT : On 'Installation type' select 'Something else'
  - RECOMMENDED : Partition order on disk should be EFI, Swap, Root at beginning of disk; Home at end of disk
  - RECOMMENDED : Swap partition equal to expected Max RAM (32GB)
  - RECOMMENDED : Root partition must support OS and Installed Apps (100GB)
  - Setup Swap, Root & Home partitions as necessary
  - IMPORTANT : Ensure that your USB Device is selected for 'Device for boot loader installation'
  - Complete the Installer Wizard

## Setting up OS Virtual Machine
  - Run VirtualBox
  - Create a New VM for the appropriate OS (this is our USB-OS VM)
  - Setup the System Settings as usual
  - IMPORTANT : On 'Hard disk' select 'Do not add a virtual hard disk'
  - Open Settings for the USB-OS VM
  - IMPORTANT : Under 'System Settings' check 'Enable EFI'
  - Update any System Settings necessary
  - Under Network setup a Bridged Adapter
  - Under USB Select the appropriate USB Controller & Add the USB Device to the Filters
  - Click OK Button

## Booting OS Virtual Machine
  - Run VirtualBox
  - Start the USB-OS VM created above
  - Exit the shell using 'exit'
  - Select 'Boot Manager'
  - Select 'EFI USB Device'
  - Wait for OS to boot and login as usual

## Booting Bare Metal
  - Connect the USB Device to a UEFI Compatable Machine
  - Turn on the Machine
  - Ensure that the Machine Boots from the USB Device (most Machines use F2 for BIOS Setup and F12 for Boot Options)
  - Wait for OS to boot and login as usual
