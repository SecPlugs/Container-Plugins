#ifndef SECPLUGS_COMMON_DOCKERPLUGIN_H
#define SECPLUGS_COMMON_DOCKERPLUGIN_H

#include <string>
#include <utility>
#include "config/configloader.h"
#include "filewatcher/filewatcher.h"
#include "rest/restclient.h"
#include <Poco/Util/ServerApplication.h>

namespace secplugs {
    class dockerplugin : public Poco::Util::ServerApplication {
    public:
        explicit dockerplugin(std::string configfile) : configfile(std::move(configfile)) {}
        int main(const std::vector<std::string> & args) override;
        void defineOptions(Poco::Util::OptionSet& options) override;
        void handleOption(const std::string& name, const std::string& value) override;
        void initialize(Poco::Util::ServerApplication& app);
    private:
        std::string configfile;

    };
}

#endif //SECPLUGS_COMMON_DOCKERPLUGIN_H
