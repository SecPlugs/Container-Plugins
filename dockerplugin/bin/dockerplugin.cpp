#include "dockerplugin.h"

namespace secplugs {
    int dockerplugin::main(const std::vector<std::string> & args) {
        secplugs::configloader cfg(this->config().getString("config"));
        secplugs::restclient rest(cfg);
        secplugs::filewatcher watcher;
        for (auto& dir: cfg.get_watchers()) {
            watcher.add_directory(dir);
        }
        watcher.watch(rest);
        return 0;
    }

    void dockerplugin::defineOptions(Poco::Util::OptionSet& options) {
        Poco::Util::ServerApplication::defineOptions(options);
        options.addOption(
                Poco::Util::Option("config", "c", "The configuration JSON" )
                .required(true)
                .repeatable(false)
                .argument("configfile", true)
                );
    }

    void dockerplugin::handleOption(const std::string &name, const std::string &value) {
        this->config().setString(name, value);
    }

    void dockerplugin::initialize(Poco::Util::ServerApplication &app) {
        std::cout << "Init App" << '\n';
        this->config().setString("config", "");
        this->loadConfiguration();
        Poco::Util::ServerApplication::initialize(app);
    }
}