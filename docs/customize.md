Customizing
===========
Here are some point on how to customize hikogui-hello-world
as a start for your own application.

Metadata
--------
There are two main files to alter for the metadata of the project.

### vcpkg.json
The vcpkg.json is the manifest file for the vcpkg system and
is read by CMake to fill in several templates.

The following fields can be configured:

 - `name`: The slug-name of the program, should only contain lower
   case letters and dashes.
 - `version`: The version number of the program
   should be semantic version number.
 - `license`: The license the program is distributed under.
 - `home`: The homepage for the program
 - `dependecies`: A list of dependencies. Since this is an hikogui
   application 'hikogui' should be part of the dependencies.

### package.json.in
The package.json.in includes metadata information needed for creating
an installer for the program. Several fields will be filled in by CMake
from vcpkg.json, and in turn is read back by CMake to fill in other
templates.

 - `display-name`: The pretty name of the program, may contain
   captial letters and spaces. On Windows it is used in
   for directory names and registry keys.
 - `vendor`: The name of the vendor. On Windows it is used
   for the directory name in C:\Program Files.
 - `signtool`: The command-line to use for signing the program(s),
   the uninstaller and installer on windows.
   Use double-quotes around arguments that may contain spaces.
   There are three place holders:
    - `{signtool}`: Replaced by the location of the signtool.exe excutable.
    - `{executable}`: Replaced by the filename of the executable to be signed.
    - `{password}`: Replaced by the password entered during [ecpack-and-sign]
      tool.

### data/cpack
The data/cpack directory contains files for customizing the installer.
See the [ecpack-and-sign] project for information to configure these
files.

[ecpack-and-sign]: https://github.com/hikogui/ecpack-and-sign

