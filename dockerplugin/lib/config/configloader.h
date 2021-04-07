#include <utility>
#ifndef SECPLUGS_COMMON_CONFIGLOADER_H
#define SECPLUGS_COMMON_CONFIGLOADER_H

#include <fstream>
#include <string>
#include <vector>
#include <Poco/JSON/Parser.h>
#include <filesystem>

#define SECPLUGS_API_URL "api.live.secplugs.com"
#define DEFAULT_API_KEY "Y5xkyVJjeh2odCl5D9nZM2xNPFCViVl04UnV17KS"

using stringpair_t = std::pair<std::string, std::string>;

namespace secplugs {
    class configloader {
    public:
        explicit configloader(std::string  file);
        configloader() = default;
        void load_config(std::string file);
        [[nodiscard]] inline std::string get_api_key() const { return api_key; }
        [[nodiscard]] inline std::string get_base_url() const { return base_url; }
        [[nodiscard]] inline std::vector<std::string> get_watchers() const { return watchers; }
        [[nodiscard]] inline std::string get_client_cert() const { return client_cert; }
        [[nodiscard]] inline std::string get_scheme() const {return scheme;}

    private:
        std::string config_file;
        std::string api_key;
        std::vector<std::string> watchers;
        std::string base_url;
        std::string scheme = "https://";
        std::map<std::string, std::string> proxy;
        std::string client_cert;
    };
}

#endif //SECPLUGS_COMMON_CONFIGLOADER_H
