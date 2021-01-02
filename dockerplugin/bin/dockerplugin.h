#ifndef SECPLUGS_COMMON_DOCKERPLUGIN_H
#define SECPLUGS_COMMON_DOCKERPLUGIN_H

#include <string>
#include "config/configloader.h"
#include "filewatcher/filewatcher.h"
#include "rest/restclient.h"
#include <Poco/Util/ServerApplication.h>

namespace secplugs {
    class dockerplugin : public Poco::Util::ServerApplication {
    public:
        dockerplugin(const std::string &configfile) : configfile(configfile) {}

        int run(int, char **);

    private:
        std::string configfile;

    };
}

#endif //SECPLUGS_COMMON_DOCKERPLUGIN_H
