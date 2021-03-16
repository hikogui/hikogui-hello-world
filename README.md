[![Build status](https://ci.appveyor.com/api/projects/status/ffurx65a8eg5fluo/branch/main?svg=true)](https://ci.appveyor.com/project/takev/ttauri-hello-world/branch/main)

ttauri\_hello\_world
====================
A minimal example of a ttauri application.

![Screenshot](docs/media/screenshot-20210218.gif)

Features
--------
 - Installs the ttauri library through vcpkg.
 - Translations in English and Dutch.
 - Dark and Light theme.
 - Easy to [customize](docs/customize.md).

Installation
------------

### MSVC - Windows 10

Install requirements:
 - The latest Microsoft Visual Studio from <https://visualstudio.microsoft.com/>
   with the following options installed:
   - C++ core desktop features
   - C++ CMake tools for Windows
   - Test Adapter for Google Test
   - Windows 10 SDK
 - git from: <https://git-scm.com/>
 - The Vulkan SDK from: <https://www.lunarg.com/vulkan-sdk/>
 - vcpkg, currently a custom version from: <https://github.com/ttauri-project/vcpkg>
 - optional: RenderDoc (for Vulkan debugging) from: <https://renderdoc.org/>
 - optional: Doxygen (for documentation generation) from: <https://www.doxygen.nl/>
 - optional: nullsoft scriptable install system from: <https://nsis.sourceforge.io/Download>

### Install vcpkg

_Note: currently we require a custom version of vcpkg which includes the port to ttauri_

From the "Developer Command Prompt for VS 2019":
```no_syntax
c:\> cd c:\tools
c:\tools>git clone git@github.com:ttauri-project/vcpkg.git
Cloning into 'vcpkg'...
...
```

Make sure we have checked out "ttauri-port"
```
c:\tools>cd vcpkg
c:\tools\vcpkg>git status
On branch ttauri-port
...
```

Get vcpkg and visual studio configured for vcpkg:
```
c:\tools\vcpkg>bootstrap-vcpkg.bat
Downloading https://github.com/microsoft/vcpkg-tool/releases/download/2021-02-24-d67989bce1043b98092ac45996a8230a059a2d7e/vcpkg.exe -> C:\tools\vcpkg/vcpkg.exe
Done.
...

c:\tools\vcpkg>vcpkg.exe integrate install --feature-flags=manifests
Applied user-wide integration for this vcpkg root.
...
```

### Clone this project

Clone the ttauri_hello_world repository on your machine:

```no_syntax
c:\Users\Tjienta\Projects>git clone git@github.com:ttauri-project/ttauri_hello_world.git
```

### Building and running with Visual Studio

You can then open the ttauri\_hello\_world directory as a [directory-based project]
inside visual studio.

To build:
 1. Select `x64-MSVC-Debug` from the project `Configuration` pull down menu.
 2. `Project / Generate Cache` menu option
 3. `Build / Build All` menu option
 4. Select `hello_world.exe` from the `Select Startup Item...` pull-down menu.
 5. `Debug / Start Debugging`

_Note: A "Window Security Alert" may show up, this is due to the RenderDoc API
creating a network server so that the RenderDoc application can remotely communicate
to the hello\_world application._

### Debugging using RenderDoc

The debug build of hello\_world.exe starts a RenderDoc-server. After starting
hello\_world.exe start the RenderDoc application.

_Note: Since ttauri renders only on changes RenderDoc may not show the application,
or react until you force the hello\_world.exe to redraw. Forcing redraw can
be done bringing the hello\_world.exe window forward, or hovering over with
your mouse._

Select `File / Attach to running instance` It would show `hello\_world` underneath `localhost`.

Now you can `Capture Frame(s) Immediately` and select the frame for debugging
the drawing.

[directory-based project]: https://docs.microsoft.com/en-us/visualstudio/ide/develop-code-in-visual-studio-without-projects-or-solutions?view=vs-2019
