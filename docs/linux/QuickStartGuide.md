
A brief documentation on how to install and use the secplugs-linux-oas-plugin

### Overview

The _{sp} linux OnAccess Scanner plugin_ is a {sp} plugin that does On-Access scan
for Linux distributions. There are official packages for Debian, Ubuntu, Fedora,
CentOS and Alpine. This plugin is a 64-bit native binary that is created with
efficient detections and low memory footprint in mind to ensure that the scan is both quick and doesn't cause a lag on your system. This document shows how this
plugin can be used from all the supported Linux distributions

### Debian/Ubuntu

The following commands will install the secplugs-linux-oas-plugin

```console
apt install software-properties-common && apt update
apt-add-repository -y ppa:secplugs/ppa
apt update && apt install secplugs-linux-oas-plugin
```

The above commands add the secplugs ppa repository to the list of repositories. The `apt-add-repository` utility needs `software-properties-common` and hence that gets installed first.

Likewise, for Debian, the following commands can be used

```console
apt install software-properties-common && apt update
apt-key adv --keyserver <keyserver> --recv <key> && apt-add-repository -y 'deb https://packages.secplugs.com/debian/ stable main'
apt update && apt install secplugs-linux-oas-plugin
```

The above adds the secplugs debian repository and its gpg keys to the system.

Both ubuntu and debian packages add the OnAccess scanner plugin to be run in the
background as a systemd service

### Fedora/CentOS

The following commands will install the plugin on Fedora/CentOS and other RPM based Linux distributions.

```console
wget -O /etc/yum.repos.d/secplugs.repo https://packages.secplugs.com/fedora/secplugs.repo
echo <SHA> /etc/yum.repos.d/secplugs.repo | sha256sum --check --status
dnf update && dnf install secplugs-linux-oas-plugin
```


### Configuration

The only configuration that is needed for the secplugs-linux-oas-plugin is the
`/etc/secplugs/config.json` file. For most common use cases, the only configuration that would be needed is the `watchers` property which tells the plugin which folders to monitor. The default configuration works for most common deployments

```console
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
```