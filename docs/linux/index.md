---
layout: none
---

# Linux File Watcher
Designed to be installed on to Linux Systems, this {brand-plugin-type} will monitor and scan file system activity in locations that you can specify.

With the code up on GitHub the {brand-plugin-type} is readly extensible but can also be used out of the box with default configuration suitable for common scenarios.

### Overview

The Linux OnAccess Scanner plugin_ is a {brand-plugin-type} that does On-Access scan
for Linux distributions. There are official packages for Debian, Ubuntu, Fedora,
CentOS and Alpine. This {brand-plugin-type} is a 64-bit native binary that is created with
efficient detections and low memory footprint in mind to ensure that the scan is both quick and doesn't cause a lag on your system. This document shows how this
plugin can be used from all the supported Linux distributions

### Debian/Ubuntu

The following commands will install the secplugs-linux-oas-plugin

```sh
apt install software-properties-common && apt update
apt-add-repository -y ppa:secplugs/ppa
apt update && apt install secplugs-linux-oas-plugin
```

The above commands add the secplugs ppa repository to the list of repositories. The `apt-add-repository` utility needs `software-properties-common` and hence that gets installed first.

Likewise, for Debian, the following commands can be used

```sh
apt install software-properties-common && apt update
apt-key adv --keyserver <keyserver> --recv <key> && apt-add-repository -y 'deb https://packages.secplugs.com/debian/ stable main'
apt update && apt install secplugs-linux-oas-plugin
```

The above adds the secplugs debian repository and its gpg keys to the system.

Both ubuntu and debian packages add the OnAccess scanner {brand-plugin-type} to be run in the
background as a systemd service

### Fedora/CentOS

The following commands will install the {brand-plugin-type} on Fedora/CentOS and other RPM based Linux distributions.

```sh
wget -O /etc/yum.repos.d/secplugs.repo https://packages.secplugs.com/fedora/secplugs.repo
echo <SHA> /etc/yum.repos.d/secplugs.repo | sha256sum --check --status
dnf update && dnf install secplugs-linux-oas-plugin
```


### Configuration

The only configuration that is needed for the secplugs-linux-oas-plugin is the
`/etc/secplugs/config.json` file. For most common use cases, the only configuration that would be needed is the `watchers` property which tells the {brand-plugin-type} which folders to monitor. The default configuration works for most common deployments

```json
{
    "watchers": [
        "/lib",
        "/lib64",
        "/var/lib",
        "/etc/"
    ],
      "vendor": "<preferred vendor from vendor list in portal>"
}
```

### Use Your Own API Key
To use additional features and the privacy of your own account, after registering with {brand-name}, sign in with your username and [create an API key](docs?doc=docs/HowTo/CreateKey) 

After creating a key, specify it as the key to use in the json configuration

```json
{
    "watchers": [
        "/lib",
        "/lib64",
        "/var/lib",
        "/etc/"
    ],
      "vendor": "<preferred vendor from vendor list in portal>",
      "api_key": "<your api key>"
}
```

## Contact
Having trouble? [Contact {brand-name} ](https://{brand-root-domain}/contacts)