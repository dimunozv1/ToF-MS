# xTDC4 User Guide Example

## About

### Overview
This directory contains a project to compile the C++ example show in the user guide of the cronologic xTDC4 time-to-digital converter.

You can download the user guide from the [product web page](https://www.cronologic.de/products/tdcs/xtdc4-pcie).

### Copyright and Licence
This code is created entirely by cronologic GmbH & Co. KG. All rights reserved.

### Supported Environments
| Platform     | Architecture | Configuration |
| ------------ | ------------ | ------------- |
| Windows  | `x64` | Debug   |
|          | `x64` | Release |
| Linux    | `x64` | Debug   |
|          | `x64` | Release |

## Build and Run the Project on Windows

### Prerequisites
1. Inf not already found, copy the `driver` folder from the `xTDC4` installation folder (e.g. on <_C:\Program Files\cronologic\xTDC4_>) to the `xTDC4_user_guide_example` folder. If the driver not already installed, you can install it from [product web pag](https://www.cronologic.de/products/tdcs/xtdc4-pcie).
2. [`CMake`](https://cmake.org/install/) is installed.

### 1. Build Using Visual Studio
- The project is built by using `Visual Studio "C++ CMake Tools` that reads speciificaitions got from `tools\CMakeSettings.json`, and configures `CMakeLists.txt`, then  builds the generated buildsystem files.
- `CMakeSettings.json` and the steps mentioned here are compatible with Visual Studio 2022. 

#### Additional Prerequisites
1. Visual Studio 2022 is installed.
2. [Visual Studio "C++ CMake Tools"](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio) is installed.

#### Open the Project 
1. Open Visual Studio.
2. Select `Open a local folder`.
3. Select the project folder, i.e. `xTDC4_user_guide_example`.
4. If "CMake Integration" message _to enable Visual Studio's CMake support with this workspace_ is displayed
   1. Select `Enable and set source directory` button.
   2. In "Select CMakeLists.txt" Dialog, navigate to `xTDC4_user_guide_example\tools` folder, and open our project `CMakeLists.txt` file, i.e. `xTDC4_user_guide_example\tools\CMakeLists.txt`
N.B. You can debug it normally as well from Visual Studio.

#### Configure `CMake`
Configure `CMake` to create _Project Buildsystem_ files, either:
1. Select the underlying configuration from the `Configurations` drop-box in the Visual Studio toolbar, which will automatically configure `CMake`.
2. Or, select `Project -> Configure cache` from menu.

#### Compile and Link
Select `Build -> Build All` from menu, or any standard Visual Studio way to build the project.

### 2. Build Using `CMake` Command line

#### Configure `CMake`
To configure `CMake` to create _Project Buildsystem_ files, go to tools: `cd tools`, then run the following command:
| Platform | Configurations     | Sample Command                    | 
| -------- | --------------     |---------------------------------- |
| Win64    | `Debug`, `Release` | `cmake -B ..\build\bf -A x64`     | 

#### Compile and Link
Build the project using `CMake`.
| Platform | Configuration | Sample Command                           | 
| ------   | ------------  |----------------------------------------- |
| `x64`    | Debug   | `cmake --build ..\build\bf  --config Debug`    |
| `x64`    | Release | `cmake --build ..\build\bf  --config Release`  | 

### Run the Example
#### Prerequisites
1. `cronologic_windows_kernel` is installed on the machine.
2. Device is installed properly on the machine.

#### Run
1. Copy the driver DLL to `lib\x64\<config>`.
2. Run the executable file found on `lib\x64\<config>` folder as `xtdc4_ugex.exe`.

---

## Build and Run the Project on Linux

### Prerequisites
#### Ubuntu
1. System is updated, e.g. `sudo apt-get update -y`, `sudo apt update`, then `sudo apt upgrade`.
2. Install Development Tools and `CMake`, e.g. `sudo apt-get install g++ cmake`.
3. `libxtdc4_driver.so` is copied to `xTDC4_user_guide_example/driver/x64`, and `xTDC4` include folder is copied to `xTDC4_user_guide_example/driver/include`. All are already found on the repository, and you can get them both from the support team if needed.

### Using `CMake`
#### Configure `CMake`
To configure `CMake` to create _Project Buildsystem_ files, go to tools: `cd tools`, then run the following command:
| Configuration  | Sample Command                    | 
| -------------- |---------------------------------- |
| `Debug`        | `cmake -B ../build/bfD -DCMAKE_BUILD_TYPE=Debug`   | 
| `Release`      | `cmake -B ../build/bfR -DCMAKE_BUILD_TYPE=Release` | 

#### Compile and Link
Build the project using `CMake`.
| Configuration | Sample Command         | 
| ------------  |----------------------  |
| Debug   | `cmake --build ../build/bfD` |
| Release | `cmake --build ../build/bfR` | 
* The code is linked to the shared library `libxtdc4_driver.so`.

### Run the Example
#### Prerequisites
1. `Cronologic PCI Linux Kernel Module` is installed on the machine. You can build it from [`cronologic_linux_kernel`](https://github.com/cronologic-de/cronologic_linux_kernel) on github.
2. Device is installed properly on the machine.

#### Run
Run the executable file found on `lib/x64` folder as `sudo ./xtdc4_ugex`.

