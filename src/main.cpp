
#include <ttauri/application.hpp>
#include <ttauri/application_delegate.hpp>
#include <ttauri/GUI/gui_system.hpp>
#include <ttauri/GUI/gui_system_delegate.hpp>
#include <ttauri/GUI/gui_window_delegate.hpp>
#include <ttauri/widgets/widgets.hpp>
#include <ttauri/crt.hpp>
#include <memory>

namespace hello {

class main_window_controller : public tt::gui_window_delegate {
public:
    void init(tt::gui_window &sender) noexcept override
    {
        using namespace tt;

        sender.make_widget<label_widget, "L0T0"_ca>(l10n("Hello:"));

        auto radio1 = sender.make_widget<radio_button_widget<int>, "L+1"_ca>(0, _value);
        radio1->label = l10n("World");

        auto radio2 = sender.make_widget<radio_button_widget<int>, "T+1"_ca>(1, _value);
        radio2->label = l10n("Universe");
    }

private:
    tt::observable<int> _value;
};

class application_controller :
    public std::enable_shared_from_this<application_controller>,
    public tt::application_delegate,
    public tt::gui_system_delegate {
public:
    static inline std::weak_ptr<application_controller> global;

    application_controller() noexcept
    {
        _main_window_controller = std::make_shared<main_window_controller>();
    }

    std::string application_name(tt::application &sender) const noexcept override
    {
        return std::string{"Hello World"};
    }

    /** The delegate to be used for the gui system.
     * @return The delegate to be used for the gui system, or empty if the gui system should not be initialized.
     */
    virtual std::weak_ptr<tt::gui_system_delegate> gui_system_delegate(tt::application &sender) noexcept
    {
        return weak_from_this();
    }

    std::optional<int> main(tt::application &sender) override
    {
        using namespace tt;
        gui_system::global->makeWindow(_main_window_controller, label{URL{"resource:hello_world.png"}, l10n("Hello World")});
        return {};
    }

    void last_window_closed(tt::gui_system &sender) override {
        using namespace tt;
        application::global->exit();
    }

private:
    std::shared_ptr<main_window_controller> _main_window_controller;
};

} // namespace hello 

int tt_main(std::vector<std::string> arguments, tt::os_handle instance)
{
    auto application_controller = std::make_shared<hello::application_controller>();
    hello::application_controller::global = application_controller;

    auto app = tt_application(application_controller, std::move(arguments), instance);
    return app.main();
}
