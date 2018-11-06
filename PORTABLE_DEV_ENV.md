# Installing OS to a USB Drive

## Requirements
  - VirtualBox
  - OS Installer ISO
  - DiskGenius
  - Large Blank USB Drive (65GB minimum for OS)

## Creating EFI/ESP Partition
  - Run DiskGenius
  - Click on the USB Drive from the list on the left
  - Right click in the 'Partitions' list
  - Select 'ESP/MSR Partition'
  - Adjust the size of the ESP Partition if needed (between 100-500MB)
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
  - Update any System Settings necessary
  - Under Storage set the OS Installer Disk ISO as the CD Drive
  - Under Network setup a Bridged Adapter
  - Under USB Select the appropriate USB Controller & Add the USB Device to the Filters
  - Click OK Button
  
## Installing the OS
  - Start the Installer VM
  - Launch the OS Installer
  - Follow the Installer Wizard
  - On 'Updates and other software' check 'Install third-party software for ...'
  - IMPORTANT : On 'Installation type' select 'Something else'
  - Setup Swap, Root & Home partitions as necessary
  - IMPORTANT : Ensure that your USB Device is selected for 'Device for boot loader installation'
  - Complete the Installer Wizard

## Launching the OS from VirtualBox
  - Identify the Disk Number of the USB Drive using Disk Management
  - Open Command Prompt as Admin
  - Run : cd %programfiles%\Oracle\VirtualBox
  - Run : VBoxManage internalcommands createrawvmdk -filename C:\usb.vmdk -rawdisk \\.\PhysicalDrive<DiskNumber>
  - Move and rename the file C:\usb.vmdk as appropriate; this is a Virtual Drive to our USB Drive
  - Run VirtualBox as Admin
  - Create a New VM for the appropriate OS (this is our Installer VM)
  - Setup the System Settings as usual
  - IMPORTANT : On 'Hard disk' select 'Use an existing virtual hard disk file'
  - Select the vmdk file created above
  - Open Settings for the Installer VM
  - Update any System Settings necessary
  - Under Network setup a Bridged Adapter
  - Under USB Select the appropriate USB Controller & Add the USB Device to the Filters
  - Click OK Button
  - Start the Virtual Machine
  - If dumped into UEFI Shell, exit the shell, select 'Boot Manager' and select 'EFI USB Device'
  - Wait for OS to boot and login as usual

## Booting Bare Metal
  - Connect the USB Device to a UEFI Compatable Machine
  - Turn on the Machine
  - Ensure that the Machine Boots from the USB Device (most Machines use F2 for BIOS Setup and F12 for Boot Options)
  - If dumped into UEFI Shell, exit the shell, select 'Boot Manager' and select 'EFI USB Device'
  - Wait for OS to boot and login as usual
