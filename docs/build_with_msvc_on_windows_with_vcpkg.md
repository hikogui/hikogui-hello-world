Building with MSVC on Windows with vcpkg
========================================

Install vcpkg
-------------

```sh
mkdir C:\tools
cd C:\tools
git clone git@github.com:microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

Clone the hikogui-hello-world
-----------------------------
Clone the hikogui-hello-world repository on your machine:

```sh
git clone git@github.com:hikogui/hikogui-hello-world.git
```

Building hikogui-hello-world
----------------------------
We expect that hikogui is located in the same directory as hikogui-hello-world
if you install hikogui somewhere else you will need to change
`CMAKE_PREFIX_PATH` in the commands bellow.

Use the *x64 Native Tools Command Prompt for VS 2022 Preview* as the command
prompt as it will configure the environment variables which are needed for
CMake to build using MSVC.

```sh
cd hikogui-hello-world
mkdir build
cd build

cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=..\install -DCMAKE_TOOLCHAIN_FILE=c:\tools\vcpkg\scripts\buildsystems\vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows ..

cmake --build .

cmake --install .
```

Packaging hikogui-hello-world
------------------------------
You can also create a cpack package which can be used to create
a package with hikogui/ecpack-and-sign. This needs to be done
after `cmake --build .`

```sh
cpack .
```
