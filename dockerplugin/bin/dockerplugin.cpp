//
// Created by bala on 31/12/20.
//

#include "dockerplugin.h"

namespace secplugs {
    int dockerplugin::run(int argc, char** argv) {
        secplugs::configloader cfg(configfile);
        secplugs::restclient rest(cfg);
        secplugs::filewatcher watcher;
        for (auto& dir: cfg.get_watchers()) {
            watcher.add_directory(dir);
        }
        watcher.watch(rest);
        return 0;
    }
}