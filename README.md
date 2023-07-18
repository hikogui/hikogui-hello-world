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

You will need to following tools to create an installable package:
 - ecpack-and-sign from: <https://github.com/hikogui/ecpack-and-sign>
 - nullsoft scriptable install system from: <https://nsis.sourceforge.io/Download>

The .github/workflows/build-on-win64.yml creates a `*-ecpack.zip` file, which is consumed
by *ecpack-and-sign*. If you want to build this `*-ecpack.zip` without github actions,
you can read the recipe from the workflow file.

First download or create the `*-ecpack.zip` file in this example it is stored in
`%USERPROFILE%\Downloads`

```
git clone https://github.com/hikogui/ecpack-and-sign
mkdir ecpack-and-sign\build
cd ecpack-and-sign\build
python ../ecpack-nsis.py %USERPROFILE%\Downloads\hikogui-hello-world-0.7.0-win64-ecpack.zip
```

This will create an installer in the `build` dir and sign all the executables, it may request
you to enter the password of your executable sign-key and you may need
to answer User Account Control dialogs.

