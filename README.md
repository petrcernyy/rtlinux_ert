# Real-Time Linux Custom Target for Simulink (`rtlinux_ert`)

This repository contains a custom System Target File (STF) for Simulink.

## Prerequisites
* Windows 10 or Windows 11
* MATLAB & Simulink (with Embedded Coder / Simulink Coder installed)
* A target device running Real-Time Linux (PREEMPT_RT) reachable via SSH

---

## Environment Setup (Windows & WSL)

### 1. Install WSL 2
If you do not have Windows Subsystem for Linux installed, open **PowerShell as Administrator** and run:
`wsl --install`

*Restart your computer when prompted.* This will install the default Ubuntu distribution. Open the "Ubuntu" app from your Start Menu to set up your UNIX username and password.

### 2. Install Required Linux Packages
Once inside your WSL Ubuntu terminal, update your package lists and install the build dependencies required by the deployment hook:
`sudo apt update && sudo apt upgrade -y`
`sudo apt install -y cmake build-essential sshpass ninja-build pkg-config`

### 3. Install the Yocto SDK
Extract or install your Yocto cross-compilation SDK inside your WSL filesystem (e.g., in your `/home/<username>/` directory). Make a note of the exact path to the environment setup script.
* Example: `/home/user/yocto-sdk/environment-setup-cortexa76-poky-linux`

---

## MATLAB Target Installation

Now that the Linux environment is ready, you need to register the custom target inside MATLAB.

1. Download or clone this repository to your Windows machine (preferably on your `C:\` drive to avoid WSL network drive mounting issues).
2. Open MATLAB and navigate to the root folder of this repository.
3. In the MATLAB Command Window, run the setup script:
   `install`
4. The script will automatically add the necessary `codegen` and `blocks` folders to your MATLAB path. 
5. When prompted, paste the absolute path to your Yocto SDK environment script inside WSL.
