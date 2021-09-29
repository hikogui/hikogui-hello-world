// Copyright Take Vos 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

// Include the gui_system to be able to create a new window.
#include <ttauri/GUI/gui_system.hpp>

// Include the radio button widget.
#include <ttauri/widgets/radio_button_widget.hpp>

// The ttauri/crt.hpp provides the main() and WinMain() functions and will
// call tt_main(). It should only be included in a single compilation unit.
#include <ttauri/crt.hpp>

// The metadata.hpp was created from the template metadata.hpp.in by CMake.
#include "metadata.hpp"

// tt_main() is a portable entry point for a ttauri application. The arguments
// are split and escaped according to the specification of the operating system
// and are properly UTF-8 encoded.
//
// tt_main() is called by main() or WinMain() from the
// `#include <ttauri/crt.hpp>` header.
int tt_main(int argc, char *argv[])
{
    // Configures the metadata of the application, this is an optional step.
    initialize_application_metadata();

    // This is an integer variable that can be observed and modified by multiple
    // widgets.
    //
    // Although not shown in this example, observable variables can also be used
    // by other objects such as a persistent preferences which will
    // automatically store the preferences to a file each time the variable
    // changes in value.
    tt::observable<int> foo;

    // Create a GUI system object which will own the main event loop and any
    // windows we create.
    auto gui = tt::gui_system::make_unique();

    // The window here is created by the GUI system.
    // 
    // The label is both a text and icon to be shown by the operating system and
    // inside the toolbar of the window.
    //
    // The special URL-scheme "resource:" is used to indicate that the location
    // of the file is part of the application resources, these resources can be
    // located in different places depending on the operating system. It is even
    // possible to include a resource directly in the executable's binary.
    auto &window = gui->make_window(tt::label{tt::URL{"resource:hello_world.png"}, tt::l10n("Hello World")});

    // The `make_widget()` function instantiates a widget inside the window's
    // content, which is a `tt::grid_layout_widget`.
    //
    // The first argument is used as a cell position on the
    // `grid_layout_widget`. The string is a spread-sheet coordinate, with the
    // column in letters from left-to-right, and the row number from top to
    // bottom. The most left top cell is "A1".
    //
    // The rest of the arguments is passed to the constructor of the
    // `label_widget`. The first and only argument is a localizable text
    // `l10n("Hello:")`, The widget will select the translated text during
    // rendering and will update when the language of the operating system is
    // changed.
    //
    // There is a scripts/create_pot.sh which will use the `gettext` application
    // to extract all string-literals inside l10n() function calls.
    window.content().make_widget<tt::label_widget>("A1", tt::l10n("Hello:"));

    // Create a radio button widget at "B1", the second column from the left on
    // the first row, the same row as the "Hello" label.
    //
    // The third argument is the value to observe or update; radio buttons are
    // able to handle many types of values, including custom ones. In this case
    // we are observing an `int` value.
    //
    // The forth argument is the value that the radio button considers to be
    // "on". When the radio button is activated, it will set `foo` to `0`.
    //
    window.content().make_widget<tt::radio_button_widget>("B1", tt::l10n("World"), foo, 0);

    // Create a second radio button widget, below the first one
    //
    // As you can see this second radio button observes the same `foo`, which
    // allows the two radio buttons to work together to toggle the value
    // between `0` and `1`.
    //
    // This radio button is considered "on" when the `int` value is `1`.
    //
    window.content().make_widget<tt::radio_button_widget>("B2", tt::l10n("Universe"), foo, 1);

    // Start the event loop, until all windows are closed. Or until
    // `exit()` is called on the gui object.
    return gui->loop();
}
