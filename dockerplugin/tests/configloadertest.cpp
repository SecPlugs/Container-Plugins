#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "config/configloader.h"
#include <string>

SCENARIO("Tests for config loading", "[config]") {
    GIVEN("A valid config file") {
        secplugs::configloader cfg;

        WHEN("the config file is loaded") {
            cfg.load_config("data/valid.json");
            THEN("the config_loaded property should be true") {
                REQUIRE(cfg.config_loaded());
            }
            AND_THEN("accessing the config properties should be successful") {
                auto api = cfg.get<std::string>("api_key");
                REQUIRE(api == "test1234");
            }
            /*AND_THEN("accessing multi-valued properties should be successful") {
                std::vector<std::string> monitors = cfg.getMany<std::string>("watchers");
                REQUIRE(monitors[0] == "/var");
            }*/
            AND_THEN("accessing configs via accessors should be successful") {
                auto api = cfg.get_value<std::string>("api_key");
                REQUIRE(api == "test1234");
            }
            AND_THEN("access configs via accessors for multi-valued config should be successful") {
                auto watchers = cfg.get_value<std::vector<std::string>>("watchers");
                REQUIRE(watchers[0] == "/var");
            }
        }
    }
}