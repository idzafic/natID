# natID (Native Interface Design for macOS, linux, and windows)

natID is a C++ framework for developing and distributing a wide range of applications, with a primary focus on engineering and data processing. It supports the following operating systems:

  - macOS 12.3+ (Intel and ARM/Apple Sillicon)
  - Linux (requires GTK 4.14 or later)
  - Windows 10, 11

On macOS, it leverages native Cocoa libraries. On Linux and Windows, it uses GTK4. Future plans include providing native support on Windows through the Windows App SDK.

## Installation

1. Download the Repository
  - Clone or download the repository from the main branch.

2. Set Up in the HOME Folder
  - Place the repository in your HOME folder.
  - The framework uses two subfolders: Work and other_bin, which must also be located in your HOME folder.
  - On Windows, ensure the HOME environment variable is defined.

3. Download Release Binaries
  - Visit the [Releases](https://github.com/idzafic/natID/releases) section and download the release binaries (other_bin_*** files).
  - Extract the binaries into the other_bin folder.
  - Prepare the Build Environment

4. Install CMake (GUI) or use an IDE that supports CMake.
  - Recommended tools for building:
    - Windows: Visual Studio 2022 Community Edition
    - macOS: Xcode
    - Linux: Qt Creator or Visual Studio Code

## Demo Projects

Demo projects are located in Work/CppProjects.
It is recommended to start with the Lectures folder and explore other projects afterward

## Selected Setups

To try prebuilt applications:

  - Download the setup package for your operating system from the [Releases](https://github.com/idzafic/natID/releases) section (SelectedSetups_*** files).
  - Extract the package and follow the installation steps:
  - On macOS, the setup is provided as an application bundle. You can run it directly or move it to the Applications folder.

## Screenshots

.![pendulum](https://github.com/user-attachments/assets/9186d2f9-1530-44d3-b8d7-9a501f3c1a4a)

![dataReporst](https://github.com/user-attachments/assets/c24a814b-03e3-494b-97ec-28d3a5d60ad6)

![snake](https://github.com/user-attachments/assets/e610e0b7-201b-4498-9364-25fd804b2645)

![navigator](https://github.com/user-attachments/assets/6ef8d036-60f3-4f82-9321-ad79075842fe)
![shapeEditor](https://github.com/user-attachments/assets/c2ad3f15-69c4-4304-ad84-bd3814606030)
![mineSweeper](https://github.com/user-attachments/assets/7f48c616-750e-42d7-b300-0e5cefc45f2f)

![glCubeMapBox](https://github.com/user-attachments/assets/5172d17c-d82c-4e95-9639-ff1eb94a20d4)
