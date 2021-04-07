#include <unistd.h>
#include "filewatcher.h"

namespace secplugs {
    /**
     * 
     * @param dir - The directory to be added to the watch list
     *
     * @return true if the dir was successfully added by inotify_add_watch, false otherwise
     */
    bool filewatcher::add_directory(const std::string & dir) {
        if (std::filesystem::exists(dir)) {
            watched_dirs.push_back(dir);
            for (auto& wdir: watched_dirs) {
                int w = inotify_add_watch(inotify_fd, wdir.c_str(), IN_CREATE | IN_MOVED_TO);
                if (w < 0) {
                    std::cerr << "Failed tp add " << wdir << " to the list of dirs being watched" << '\n';
                } else {
                    watch_map.insert(std::pair<int, std::string>(w, dir));
                    std::cout << "Added " << wdir << " to watch list" << '\n';
                }
            }
            return true;
        }
        return false;
    }

    /**
     * The default constructor initialises the inotify file descriptor and adds the directories to be
     * "watched". The watch descriptors are cached to build the complete absolute file path subsequently
     * for submitting the file.
     *
     */
    filewatcher::filewatcher() {
        watched_dirs.clear();
        new_files.clear();
        inotify_fd = inotify_init();
        if (inotify_fd < 0) {
            std::cerr << "Error initialising inotify" << '\n';
        }
        for (auto& dir: watched_dirs) {
            int w = inotify_add_watch(inotify_fd, dir.c_str(), IN_CREATE | IN_MOVED_TO);
            if (w < 0) {
                std::cerr << "Failed tp add " << dir << " to the list of dirs being watched" << '\n';
            } else {
                watch_map.insert(std::pair<int, std::string>(w, dir));
                std::cout << "Added " << dir << " to watch list" << '\n';
            }
        }
    }

    filewatcher::~filewatcher() {
        watched_dirs.clear();
        new_files.clear();
    }

    /**
     *
     * @param rest - A pre-constructed rest client object
     *
     * This method loop through the initialised inotify event buffer for all events
     * received since the last time. We get events for every folder being watched. For
     * every file that has been newly created or moved into the watched folders, the file
     * is uploaded to S3 and sent for a file score scan.
     *
     */
    [[noreturn]] void filewatcher::watch(restclient& rest) {
        char buffer[EVENT_BUFFER];
        while (true) {
            int num_events = read(inotify_fd, buffer, EVENT_BUFFER);
            int count = 0;
            if (num_events <= 0) {
                std::cout << "No new files" << '\n';
            } else {
                unsigned long i = 0;
                while (i < num_events) {
                    auto *event = (struct inotify_event *) &buffer[i];
                    if (event->len) {
                        if (event->mask & IN_CREATE || event->mask & IN_MOVED_TO) {
                            if (event->mask & IN_ISDIR) {
                                std::cout << "Ignoring directories" << '\n';
                            } else {
                                std::filesystem::path abspath = watch_map.find(event->wd)->second;
                                abspath /= event->name;
                                new_files.emplace_back(abspath.c_str());
                            }
                        }
                    }
                    i += EVENT_SIZE + event->len;
                }
                for (auto &file: new_files) {
                    if (!rest.is_clean(file, secplugs::SCORE)) {
                        std::filesystem::remove(file);
                        std::cout << "Removing file " << file << '\n';
                        count++;
                    }
                }
                new_files.clear();
            }
        }
    }
}
