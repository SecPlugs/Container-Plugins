---
layout: none
---
#### What is the Linux File Watcher?

The Linux File Watcher is a inotify-based daemon that starts at system start up and keeps running as a system service, via systemd on most distributions.

#### What are the features?

- __Lazy On Access Scanning__ - Monitor file access and automatically submit files for analysis
- __Out of Box Defaults__ - Comes with default API keys and configuation so it works out of the box
- __Configurable Watch Directories__ - Configure which directories to watch for activity
- __Infected File Deletion__ - Delete infected files
- __{brand-name} Portal__ - With a registered API key you can access all the core Secplugs features via the portal.

#### How does it work?

At startup, the daemon reads the configuration file and initialises the process with all the directories listed in the configuration.  
The daemon monitors for files created in or moved into the configured directories. When it detects a file, the file is sent to secplugs for a score based scan.  
If this scan detects this file to be malicious, the file is removed from the filesystem.
This uses a very minimal configuration that is just enough. The list of directories to monitor is the only mandatory configuration.
Other optional configurations include the vendor to use for scanning the files and the API key.

#### How do I get started?

Simply download and install the package. Details vary so check the instructions for your target Linux Distro. 

To use additional features and the privacy of your own account, after registering in Secplugs.com, login with your username and create an API key to use with the tool. 
Replace the key in the configuration file.
You can now use the Secplugs portal to configure the {brand-plugin-type} and well as view activity, run reports and do deeper retrospective threat analysis.

