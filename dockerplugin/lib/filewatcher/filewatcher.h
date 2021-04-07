#ifndef SECPLUGS_COMMON_FILEWATCHER_H
#define SECPLUGS_COMMON_FILEWATCHER_H

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <rest/restclient.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define NUM_EVENTS 1024
#define EVENT_BUFFER (NUM_EVENTS * (EVENT_SIZE + 16))

namespace secplugs {
    class filewatcher {
    public:
        filewatcher();
        ~filewatcher();
        bool add_directory(const std::string& dir);

        [[noreturn]] void watch(restclient& rest);

    private:
        std::vector<std::string> watched_dirs;
        std::map<int, std::string> watch_map;
        std::vector<std::string> new_files;
        int inotify_fd;
    };
}

#endif //SECPLUGS_COMMON_FILEWATCHER_H
