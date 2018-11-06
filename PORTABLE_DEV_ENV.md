# Installing OS to a USB Drive

## Requirements
  - VirtualBox
  - OS Installer ISO
  - DiskGenius or GParted
  - USB Drive or External Drive (82GB minimum for OS; 132GB recommended for daily driver equivalent)

## Setting up Partitions
  - IMPORTANT : EFI Partition must be within the first 2.2TB of the disk
  - IMPORTANT : EFI Partition must be FAT32 File System
  - RECOMMENDED : EFI Partition should be the first partition 
  - RECOMMENDED : EFI Partition should be between 100-500MB
  - RECOMMENDED : Order of partitions; EFI, Swap, OS-es, Shared
  - Create EFI Partition (100-500MB; FAT32; set flags 'boot' & 'esp')
  - Create Swap Partition (32GB; Swap/EXT4)
  - Create OS Partition(s) (100GB; EXT4/NTFS)
  - Create Shared Partition (1.5TB; NTFS; aligned to end of drive)
  - Format all partitions appropriately
  - NOTE : Leaving free space on the drive will allow for additional OSes to be installed onto the same drive on new partitions

## Setting up the Installer VM
  - Run VirtualBox
  - Create a New VM for the appropriate OS (this is our Installer VM)
  - Setup the System Settings as usual
  - IMPORTANT : On 'Hard disk' select 'Do not add a virtual hard disk'
  - Open Settings for the Installer VM
  - IMPORTANT : Under 'System Settings' check 'Enable EFI'
  - Update any System Settings necessary
  - Under Storage set the OS Installer Disk ISO as the CD Drive and check 'Live CD/DVD'
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
  - Map Swap & Root mounts appropriately to pre-created partitions
  - Add a mount point for the Shared Partition (/shared)
  - Format partitions if necessary
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
