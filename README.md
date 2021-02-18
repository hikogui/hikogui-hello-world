# ttauri_hello_world
A minimal example of a ttauri application.

!(Screenshot)(docs/media/screenshot-20210218.gif)

Installation
------------

### MSVC - Windows 10

Install requirements:
 - The latest Microsoft Visual Studio Preview:
   - C++ core desktop features
   - C++ CMake tools for Windows
   - Test Adapter for Google Test
   - Windows 10 SDK
 - The Vulkan SDK from: https://www.lunarg.com/vulkan-sdk/
 - the latest Python 3.x.x from: https://www.python.org/downloads/windows/
 - optional: RenderDoc (for Vulkan debugging) from: https://renderdoc.org/
 - optional: Doxygen (for documentation generation)

### Clone this project

Clone the ttauri_hello_world repository on your machine:

```no_syntax
[Tjienta]test> git clone git@github.com:ttauri-project/ttauri_hello_world.git
Cloning into 'ttauri_hello_world'...
Warning: untrusted X11 forwarding setup failed: xauth key data not generated
remote: Enumerating objects: 39, done.
remote: Counting objects: 100% (39/39), done.
remote: Compressing objects: 100% (26/26), done.
remote: Total 39 (delta 9), reused 34 (delta 8), pack-reused 0
Receiving objects: 100% (39/39), 32.00 KiB | 448.00 KiB/s, done.
Resolving deltas: 100% (9/9), done.
```

The ttauri library is included as a git submodule inside the ttauri_hello_world
directory that was created by the previous command. The following commands will
make sure the ttauri_hello_world/ttauri directory is a clone of the ttauri library
with the correct version checked out.

```no_syntax
[Tjienta]test> cd ttauri_hello_world/
[Tjienta]ttauri_hello_world> git submodule init
Submodule 'ttauri' (git@github.com:ttauri-project/ttauri.git) registered for path 'ttauri'
[Tjienta]ttauri_hello_world> git submodule update
Cloning into 'C:/Users/Tjienta/test/ttauri_hello_world/ttauri'...
Warning: untrusted X11 forwarding setup failed: xauth key data not generated
Submodule path 'ttauri': checked out '812fc9ccf4732268fb61a4b75f051045b95216bc'
```

### Building and running with Visual Studio
You can then open the ttauri\_hello\_world directory as a [directory-based project] inside visual studio.

To build, select "x64-Debug" from the project configurations, and then select
the menu option: _Build / Build All_.

To run the application select "hello\_world.exe" from the "Select Startup Item..." pull-down menu; and then press the button to run it.

A "Window Security Alert" may show up, this is due to RenderDoc creating a
network server so that the RenderDoc application can talk to the hello\_world
application.

### Debugging using RenderDoc
The debug build of hello\_world.exe starts a RenderDoc-server. After starting
hello\_world.exe start the RenderDoc application.

Since ttauri renders only on changes RenderDoc may not show the application,
or react until you force the hello\_world.exe to redraw. Forcing redraw can
be done by hovering over the hello\_world.exe window with your mouse.
You will need to do this hovering, each time you want to capture a frame.

Select _File / Attach to running instance_ It would show "hello\_world" underneath "localhost". (you may need to first do the hover-trick described above)

Now you can "Capture Frame(s) Immediately" and select the frame for debugging
the drawing. (you will need to use the hover-trick before and after clicking
this button).

[directory-based project]: https://docs.microsoft.com/en-us/visualstudio/ide/develop-code-in-visual-studio-without-projects-or-solutions?view=vs-2019
