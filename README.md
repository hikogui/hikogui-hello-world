hikogui-hello-world [![Build on Windows](https://github.com/hikogui/hikogui-hello-world/actions/workflows/build-on-win64.yml/badge.svg)](https://github.com/hikogui/hikogui-hello-world/actions/workflows/build-on-win64.yml)
====================
A minimal example of a HikoGUI application.


![Screenshot](docs/media/screenshot-20210218.gif)

Features
--------
 - Installs the HikoGUI library through vcpkg.
 - Translations in English and Dutch.
 - Dark and Light theme.
 - Easy to [customize](docs/customize.md).

Installation
------------
You can find the build instructions for different IDEs here:
 - [Visual Studio (Preview)](docs/build_with_visual_studio.md)
 - [Visual Studio (Preview) + vcpkg](docs/build_with_visual_studio_and_vcpkg.md)

Packaging
---------

 - download https://github.com/hikogui/ecpack-and-sign
 - nullsoft scriptable install system from: <https://nsis.sourceforge.io/Download>

```
cd hikogui-hello-world
cmake --build build --target package
copy "build\hikogui-hello-world-0.3.0-win64.json" "build\hikogui-hello-world-0.3.0-win64-files\ecpack.json"
zip 
```

