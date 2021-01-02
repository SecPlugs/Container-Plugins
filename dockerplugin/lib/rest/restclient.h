//
// Created by bala on 26/12/20.
//

#ifndef SECPLUGS_COMMON_RESTCLIENT_H
#define SECPLUGS_COMMON_RESTCLIENT_H

#include <utility>

#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/URI.h"
#include "Poco/Environment.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/NetException.h"
#include "Poco/URI.h"
#include "Poco/Net/FilePartSource.h"
#include "Poco/Net/StringPartSource.h"
#include "config/configloader.h"
#include "utils/common.h"

using JsonObjPtr = Poco::JSON::Object::Ptr;
using map_str_str = std::map<std::string, std::string>;

namespace secplugs {
    class restclient {
    public:
        explicit restclient(const secplugs::configloader& cfg_);
        bool is_clean(const std::string& file, const secplugs::scantype& scan_type);
    private:
        bool upload_file(const std::string & filepath, const std::string & sha256);
        bool do_scan(const std::string& sha256, const secplugs::scantype& scan_type);
        void get_presigned_url(Poco::Net::HTTPSClientSession& session, const std::string & sha256, JsonObjPtr& ptr);
        static std::string get_url(JsonObjPtr json);
        static void get_fields(JsonObjPtr json, map_str_str& fields);
        static int extractScore(const std::string& json);
        secplugs::configloader cfg;
        Poco::Net::Context::Ptr ctxt;
    };
}

#endif //SECPLUGS_COMMON_RESTCLIENT_H
