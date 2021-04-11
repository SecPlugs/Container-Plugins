= About the Container Plugin
:author: Secplugs
:toc: left
:toclevels: 2
:sp: Secplugs
:scp: secplugs-container-plugin

A brief documentation explaining the secplugs-docker-plugin

== Overview

The {scp} is a inotify-based daemon that starts at system start up and keeps running as a system service, via systemd on most distributions.

At startup, the daemon reads the configuration file and initialises the process with all the directories listed in the configuration. The daemon monitors for files created in or moved into the configured directories. When it detects a file, the file is sent to secplugs for a score based scan. If this scan detects this file to be malicious, the file is removed from the filesystem.

This uses a very minimal configuration that is just enough. The list of directories to monitor is the only mandatory configuration. Other optional configurations include the vendor to use for scanning the files, and the API key.

== Example Configuration

[code, json]
....
{
  "watchers": [
    "/lib",
    "/lib64",
    "/var/lib",
    "/etc/"
  ],
  "vendor": "<preferred vendor from vendor list in portal>",
  "api_key": "secplugs-api-key-from-portal"
}
....
