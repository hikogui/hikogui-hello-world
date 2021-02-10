// Copyright Take Vos 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <ttauri/application.hpp>
#include <ttauri/application_delegate.hpp>
#include <ttauri/GUI/gui_system.hpp>
#include <ttauri/GUI/gui_system_delegate.hpp>
#include <ttauri/GUI/gui_window_delegate.hpp>
#include <ttauri/widgets/widgets.hpp>

// The ttauri/crt.hpp provides the main() and WinMain() functions and will
// call tt_main(). It should only be included in a single compilation unit.
#include <ttauri/crt.hpp>
#include <memory>

namespace hello {

// The main_window_controller manages the main window that was opened by
// the application_controller.
// 
// As a window-delegate its methods will be called by the window at
// specific moments of its lifetime.
class main_window_controller : public tt::gui_window_delegate {
public:
    // The init() method will be called by a window after it was initialized and
    // is ready to accept widgets to be placed inside it. At this point a window
    // already contains a window_widget and inside the window_widget a toolbar_widget
    // for the toolbar and a grid_layout_widget as the content.
    // 
    // The make_widget() method will create the new widgets inside the content's
    // grid_layout_widget.
    void init(tt::gui_window &sender) noexcept override
    {
        using namespace tt;

        // Create a label widget inside the window-content's grid_layout_widget.
        // The location of the label on the grid are the absolute coordinates:
        // left=0, top=0.
        // 
        // The localizable-text (l10n) of the label is "Hello:". The label_widget
        // will select the localized text during rendering and will update when
        // the language of the operating system is changed.
        //
        // There is a scripts/create_pot.sh which will use the gettext application
        // to extract all strings-literals inside l10n() function calls.
        sender.make_widget<label_widget, "L0T0"_ca>(l10n("Hello:"));

        // Create a radio button widget, it's location is one to the right, relative to
        // the previous added widget. 'L+1' means 1 further from the left-edge.
        //
        // The template argument of the radio_button_widget, gives the type of the value
        // that the radio button will observe and update when clicked on.
        // 
        // The first argument means that the radio button is considered "on" when the
        // observed 'int' value is '0'. When you click on this radio button, the radio
        // button will set the observed value to '0'.
        //
        // The second argument is the value to observe or update.
        auto radio1 = sender.make_widget<radio_button_widget<int>, "L+1"_ca>(0, _value);
        radio1->label = l10n("World");

        // Create a second radio button widget, below the first one, using 'T+1' meaning
        // 1 further from the top-edge.
        //
        // This radio button is considered "on" when the 'int' value is '1'.
        // 
        // As you can see this second radio button observes the same value, which allows
        // the two radio buttons together to toggle the value between '0' and '1'.
        auto radio2 = sender.make_widget<radio_button_widget<int>, "T+1"_ca>(1, _value);
        radio2->label = l10n("Universe");
    }

private:
    // This is an integer variable that can be observed and modified by multiple widgets.
    //
    // Although not shown in this example, observable variables can also be used by other objects
    // such as a persistent preferences which will automatically store the preferences to a file
    // each time the variable changes in value.
    tt::observable<int> _value;
};

// An application controller manages the application through it lifetime
// It is passed to the application instance inside the tt_main() function.
//
// In this case the application controller will also be used as the gui_system_delegate
// to control the gui_system.
class application_controller :
    public std::enable_shared_from_this<application_controller>,
    public tt::application_delegate,
    public tt::gui_system_delegate {
public:
    application_controller() noexcept
    {
        // Construct the main window controller which will be used to attach
        // to the window that is created once the application is ready for new windows.
        _main_window_controller = std::make_shared<main_window_controller>();
    }

    // The application name, this name is used by the operating system and vulkan system
    // to identify the application and its windows.
    std::string application_name(tt::application &sender) const noexcept override
    {
        return "Hello World";
    }

    // The gui_system_delegate() method is used by the application to pass to the gui_system's construction.
    // In this case the application_controller itself will be used, so it will return a point of itself.
    //
    // weak_from_this() comes from the std::enable_shared_from_this<> class
    // the application_controller is inheriting from.
    std::weak_ptr<tt::gui_system_delegate> gui_system_delegate(tt::application &sender) noexcept override
    {
        return weak_from_this();
    }

    // Once most of the sub-systems of ttauri are started up the main() method is called to,
    // in this case, open the main window.
    std::optional<int> main(tt::application &sender) override
    {
        using namespace tt;

        // The main window is created here to be managed by the gui_system.
        // The gui_system will create a window and attach it to a vulkan-GPU-device.
        //
        // A window controller is passed as an argument to manage the window once it it opened.
        // The label is both a text and icon to be shown by the operating system and inside the toolbar
        // of the window.
        //
        // The special URL-scheme "resource:" is used to indicate that the location of the file is part
        // of the application resources, these resources can be located in different places depending
        // on the operating system. It is even possible to include a resource directly in the executable's binary.
        gui_system::global->make_window(_main_window_controller, label{URL{"resource:hello_world.png"}, l10n("Hello World")});

        // On a non-GUI application it is possible to return an exit code here which will cause
        // the application to tear itself down and exit.
        //
        // For a GUI application we can return empty {}, which will cause the application to enter
        // the main-loop.
        return {};
    }

    // This method is called by the gui_system when the last window is closed.
    void last_window_closed(tt::gui_system &sender) override {
        using namespace tt;

        // It is possible for an application to remain running after the last
        // window is closed, or even re-open another window.
        //
        // In this case we want to exit the application. It is possible to pass
        // an exit code to this function, the default is zero.
        application::global->exit();
    }

private:
    std::shared_ptr<main_window_controller> _main_window_controller;
};

} // namespace hello 

// tt_main() is a portable entry point for a ttauri application.
// The arguments are split and escaped according to the specification of the operating
// system and are properly UTF-8 encoded.
//
// tt_main() is called by main() or WinMain() from the `#include <ttauri/crt.hpp>`
// header. This header must be included only once in an application.
int tt_main(std::vector<std::string> arguments, tt::os_handle instance)
{
    auto application_controller = std::make_shared<hello::application_controller>();

    // Start the application.
    // The instance of this class uses RAII to manage the startup and teardown of
    // different ttauri sub systems; like the gui_system, audio_system, and logging system.
    //
    // It is important for those systems to be teared-down before the main function ends.
    auto app = tt_application(application_controller, std::move(arguments), instance);

    // Start the application.
    // The main() will in the case of a GUI application enter the main-loop.
    return app.main();
}
