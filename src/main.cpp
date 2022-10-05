// Copyright Take Vos 2021-2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

// Include the gui_system to be able to create a new window.
#include <hikogui/GUI/gui_system.hpp>

// Include the radio button widget.
#include <hikogui/widgets/radio_button_widget.hpp>

// Include png-decoder to be used for the window's icon.
#include <hikogui/codec/png.hpp>

// The hikogui/crt.hpp provides the main() and WinMain() functions and will
// call tt_main(). It should only be included in a single compilation unit.
#include <hikogui/crt.hpp>

// This provides hi::task<> which is used to support co-routines.
#include <hikogui/task.hpp>

// The metadata.hpp was created from the template metadata.hpp.in by CMake.
#include "metadata.hpp"

// This is the co-routine that will manage the main window.
hi::task<> main_window(hi::gui_system &gui)
{
    // This is an integer variable that can be observed and modified by multiple
    // widgets.
    //
    // Although not shown in this example, observable variables can also be used
    // by other objects such as a persistent preferences which will
    // automatically store the preferences to a file each time the variable
    // changes in value.
    hi::observer<int> foo;

    // The window here is created by the GUI system. It returns a shared_ptr with
    // a ref count of 1; therefor main_window() co-routine's frame is the only
    // owner of this window.
    //
    // The label is both a text and icon to be shown by the operating system and
    // inside the toolbar of the window.
    //
    // The special URL-scheme "resource:" is used to indicate that the location
    // of the file is part of the application resources, these resources can be
    // located in different places depending on the operating system. It is even
    // possible to include a resource directly in the executable's binary.
    auto icon = hi::png::load(hi::URL{"resource:hello_world.png"});
    auto window = gui.make_window(hi::label{icon, hi::tr("Hello World")});

    // The `make_widget()` function instantiates a widget inside the window's
    // content, which is a `hi::grid_layout_widget`.
    //
    // The first argument is used as a cell position on the
    // `grid_layout_widget`. The string is a spread-sheet coordinate, with the
    // column in letters from left-to-right, and the row number from top to
    // bottom. The most left top cell is "A1".
    //
    // The rest of the arguments is passed to the constructor of the
    // `label_widget`. The first and only argument is a localizable text
    // `tr("Hello:")`, The widget will select the translated text during
    // rendering and will update when the language of the operating system is
    // changed.
    //
    // There is a scripts/create_pot.sh which will use the `gettext` application
    // to extract all string-literals inside tr() function calls.
    window->content().make_widget<hi::label_widget>("A1", hi::tr("Hello:"));

    // Create a radio button widget at "B1", the second column from the left on
    // the first row, the same row as the "Hello" label.
    //
    // The second argument is the value to observe or update; radio buttons are
    // able to handle many types of values, including custom ones. In this case
    // we are observing an `int` value.
    //
    // The third argument is the value that the radio button considers to be
    // "on". When the radio button is activated, it will set `foo` to `0`.
    // 
    // The rest of the arguments are for attributes to the widget.
    // Most widgets will accept multiple types of attributes that may be specified
    // in any other. The radio button will accept labels, text styles and alignment.
    // In this case we only pass in the label "World". 
    //
    window->content().make_widget<hi::radio_button_widget>("B1", foo, 0, hi::tr("World"));

    // Create a second radio button widget, below the first one
    //
    // As you can see this second radio button observes the same `foo`, which
    // allows the two radio buttons to work together to toggle the value
    // between `0` and `1`.
    //
    // This radio button is considered "on" when the `int` value is `1`.
    //
    window->content().make_widget<hi::radio_button_widget>("B2", foo, 1, hi::tr("Universe"));

    // Wait until the window is closing. This happens when the user clicks the X in the window
    // or when the operating system request that the window should be closed in another way.
    //
    // `co_await` is a suspension-point, `tt_main()` that called `main_window()` will now continue executing.
    // Once the `loop::main().resume()` triggers the window->closing notifier, `main_window()` is resumed
    // at this point.
    co_await window->closing;

    // Falling of the edge here will destroy the co-routine's frame and cause shared_ptr to the window
    // to be destroyed.
}

// tt_main() is a portable entry point for a HikoGUI application. The arguments
// are split and escaped according to the specification of the operating system
// and are properly UTF-8 encoded.
//
// tt_main() is called by main() or WinMain() from the
// `#include <hikogui/crt.hpp>` header.
int hi_main(int argc, char *argv[])
{
    // Configures the metadata of the application, this is an optional step.
    initialize_application_metadata();

    // Create a GUI system object which will own the main event loop and any
    // windows we create.
    auto gui = hi::gui_system::make_unique();

    // Start our main window. Once the window is created main_window() will
    // give control back to hi_main().
    main_window(*gui);

    // Start the event loop, until all windows are closed.
    // Or until `exit()` is called on the gui object.
    return hi::loop::main().resume();
}
