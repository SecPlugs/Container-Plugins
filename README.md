# Container-Plugins

This repo is intended for all container related plugins

## The Docker Plugin

The docker plugin comprises of a shared library _secplugs-common_ and a binary
named _dockerplugin_. The _secplugs-common_ library provides an inotify based
file watcher and a REST client for the Secplugs API. The _dockerplugin_ binary
uses a JSON config file using which the directories to be watched can be configured.
Other configurables include the *api_key*, *base_url* etc.

### An example config

    {
      "watchers": [
        "/tmp/secplugs",
		"/var/lib",
		"/etc"
      ],
	  "api_key": "my_secplugs_api_key",
	  "vendorcfg": "hybrid_analysis"
    }

