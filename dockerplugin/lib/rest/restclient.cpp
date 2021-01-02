#include "restclient.h"

namespace secplugs {

    /**
     *
     * @param cfg_ - A pre-constructed configloader object.
     *
     * This creates the SSL context that will be common for all HTTPS sessions.
     * For now, we do not validate the certificates.
     *
     * TODO: Add a config attr for CA path and validate server certificates.
     */
    restclient::restclient(const secplugs::configloader &cfg_) {
        cfg = cfg_;
        ctxt = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE,
                                      "",
                                      Poco::Net::Context::VerificationMode::VERIFY_NONE,
                                      9,
                                      true);

        ctxt->enableExtendedCertificateVerification(false);


    }

    /**
     *
     * @param file - The file to be submitted to secplugs to be scanned
     * @param scan_type - The type of scan - Can be SCORE, QUICKSCAN or DEEPSCAN
     * @return - Returns true if the file is non-malicious, false otherwise.
     *
     * This is the only method exposed by this class for callers. This internally handles
     * uploading the file to an S3 bucket and invoking the secplugs security APIs to get
     * score/scan results.
     */
    bool restclient::is_clean(const std::string &file, const secplugs::scantype& scan_type) {
        std::string sha256 = common::compute_sha256(file);
        if (upload_file(file, sha256)) {
            return do_scan(sha256, scan_type);
        } else {
            return false;
        }
    }

    /**
     *
     * @param sha256 - The sha256 hash of the file being submitted for scan
     * @param scan_type - SCORE/QUICKSCAN/DEEPSCAN
     * @return - Returns true if the file is non-malicious, false otherwise.
     *
     * This method talks to the /file/<scantype> end point of the Secplugs security API.
     * For now, this handles only the "score" from the reponse.
     *
     * TODO: Add handlers for quick scan and deep scan results as well.
     */
    bool restclient::do_scan(const std::string& sha256, const secplugs::scantype& scan_type) {
        std::string end_point = "/security/file";
        Poco::URI uri(cfg.get_scheme() + cfg.get_base_url() + end_point);
        if (scan_type == secplugs::SCORE) {
            uri.setPath(end_point + "/score");
        } else if (scan_type == secplugs::QUICK_SCAN) {
            uri.setPath(end_point + "/quickscan");
        } else {
            uri.setPath(end_point + "/deepscan");
        }
        uri.addQueryParameter("sha256", sha256);
        uri.addQueryParameter("vendorcfg", "hybrid_analysis");
        Poco::Net::HTTPSClientSession sp_session(cfg.get_base_url(), 443, ctxt);
        Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET, uri.toString());
        req.setHost(cfg.get_base_url());
        req.add("x-api-key", cfg.get_api_key());
        req.add("Accept", "application/json");
        try {
            sp_session.sendRequest(req);
            Poco::Net::HTTPResponse res;
            std::istream &rs = sp_session.receiveResponse(res);
            if (res.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
                std::stringstream res_data;
                Poco::StreamCopier::copyStream(rs, res_data);
                int scan_score = extractScore(res_data.str());
                // score <= 20 are malicious
                std::cout << "Scan Score: " << scan_score << '\n';
                if (scan_score <= 20) {
                    return false;
                } else {
                    return true;
                }
            } else {
                std::cerr << res.getStatus() << " " << res.getReason() << '\n';
                return false;
            }
        } catch (...) {
            std::cerr << "Parse error in scan response" << '\n';
            return false;
        }
    }

    /**
     *
     * @param session - The active Poco HTTPSClientSession object
     * @param sha256 - The sha256 hash of the file that we want to upload to S3
     * @param ptr - The parameter that is populated by this method
     *
     * This method uses the /file/upload endpoint to get a pre-signed AWS URL using which
     * a file can be uploaded to S3. This endpoint returns the URL and several other HTTP params
     * that must be passed on to the URL along with the payload. The response is a JSON payload
     *
     */
    void restclient::get_presigned_url(Poco::Net::HTTPSClientSession & session, const std::string & sha256, JsonObjPtr& ptr) {
        Poco::URI uri (cfg.get_scheme() + cfg.get_base_url() + "/security/file/upload");

        uri.addQueryParameter("sha256", sha256);
        Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET, uri.toString());
        req.setHost(cfg.get_base_url());
        req.add("x-api-key", cfg.get_api_key());
        try {
            session.sendRequest(req);
            Poco::Net::HTTPResponse res;
            std::istream &rs = session.receiveResponse(res);
            std::string presigned_url;
            if (res.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
                std::stringstream res_data;
                Poco::StreamCopier::copyStream(rs, res_data);
                Poco::JSON::Parser parser;
                Poco::Dynamic::Var json_stream = parser.parse(res_data.str());
                JsonObjPtr json = json_stream.extract<Poco::JSON::Object::Ptr>();
                ptr = json->getObject("upload_post");
            } else {
                std::cerr << res.getStatus() << " " << res.getReason() <<  '\n';
                res.write(std::cerr);
            }
        } catch (...) {

        }
    }

    /**
     *
     * @param json - The JSON produced by the get_presigned_url call.
     * @return url - The URL property of the JSON that is passed as input.
     */
    std::string restclient::get_url(JsonObjPtr json) {
        if (json->has("url")) {
            return json->getValue<std::string>("url");
        } else {
            std::cerr << "No URL!!" << '\n';
        }
        return "";
    }

    /**
     *
     * @param json -  The JSON produced by the get_presigned_url call.
     * @param fields - The "fields" property of the input JSON converted to a map.
     *
     */
    void restclient::get_fields(JsonObjPtr json, map_str_str& fields) {
        auto fieldsObj = json->getObject("fields");
        for(auto itr = fieldsObj->begin(); itr != fieldsObj->end(); ++itr) {
            fields.insert(std::pair<std::string, std::string>(itr->first, itr->second));
        }
    }

    /**
     *
     * @param file - The path of the file to be uploaded
     * @param sha256 - The sha256 has of the file to be uploaded
     * @return - Indicates whether the upload was successful or not
     */
    bool restclient::upload_file(const std::string &file, const std::string &sha256) {
        Poco::Net::HTTPSClientSession sp_session(cfg.get_base_url(), 443, ctxt);
        Poco::JSON::Object::Ptr pre_signed_url = new Poco::JSON::Object;
        get_presigned_url(sp_session, sha256, pre_signed_url);
        std::string upload_uri = get_url(pre_signed_url);
        Poco::URI uri(upload_uri);
        map_str_str fields;
        get_fields(pre_signed_url, fields);
        Poco::Net::HTTPSClientSession aws_session(uri.getHost(), 443, ctxt);
        Poco::Net::HTMLForm form;
        // We will be sending several form data and a file. So we need a multipart form-data.
        form.setEncoding(Poco::Net::HTMLForm::ENCODING_MULTIPART);
        // The fields property of the JSON produced by get_presigned_url. This is needed to upload files to S3
        for(auto& [key, val]: fields) {
            form.addPart(key, new Poco::Net::StringPartSource(val));
        }
        form.addPart("file", new Poco::Net::FilePartSource(file, sha256, "application/octetstream"));
        Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_POST, uri.getPath());
        req.add("x-api-key", cfg.get_api_key());
        req.add("Host", uri.getHost());
        form.prepareSubmit(req);
        req.setVersion("HTTP/1.1");
        form.write(aws_session.sendRequest(req));
        Poco::Net::HTTPResponse res;
        std::istream & res_stream = aws_session.receiveResponse(res);
        if (res.getStatus() == Poco::Net::HTTPResponse::HTTP_OK || res.getStatus() == Poco::Net::HTTPResponse::HTTP_NO_CONTENT) {
            return true;
        } else {
            return false;
        }
    }

    /**
     *
     * @param json - The JSON response from the Secplugs file upload API
     * @return - The "score" property from the JSON response
     *
     */
    int restclient::extractScore(const std::string &json) {
        Poco::JSON::Parser parser;
        auto tmp = parser.parse(json);
        int score = tmp.extract<Poco::JSON::Object::Ptr>()->getValue<int>("score");
        return score;
    }
}
