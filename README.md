Train Ticketing System
======================

A simple command line system for tracking, placing, and managing train tickets.

Made for a semester assignment

Prerequisites
-------------

Development requires the following

- [Git](https://git-scm.com/)
- [OpenSSL](#installing-openssl)

Installing OpenSSL
------------------

### Debian

Run

```shell
sudo apt-get install openssl
```

### Windows

For compiling and building on Windows, it's recommended to use `vcpkg` to manage library.

#### Installing vcpkg

```cmd
git clone https://github.com/microsoft/vcpkg
cd vcpkg
bootstrap-vcpkg.bat
```

#### Installing required dependencies

```cmd
vcpkg search ssl
vcpkg install openssl-windows
```

In order to use vcpkg with Visual Studio,
run the following command (may require administrator elevation):

```cmd
vcpkg integrate install
```

#### Visual Studio Code with CMake Tools

Adding the following to your workspace `settings.json` will make
CMake Tools automatically use vcpkg for libraries:

```json
{
  "cmake.configureSettings": {
    "CMAKE_TOOLCHAIN_FILE": "[vcpkg root]/scripts/buildsystems/vcpkg.cmake"
  }
}
```

Getting Started
---------------

Check the [Prerequisites](#prerequisites), and install any required packages / tools.

1. Clone the repo
   ```shell
   git clone https://github.com/Poleric/train-ticketing-system.git
   ```
2. Open the folder in your IDE and configure CMake