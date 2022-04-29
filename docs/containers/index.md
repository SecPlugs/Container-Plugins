---
layout: none
---

# Docker File Watcher
Designed to be built into docker containers, this {brand-plugin-type} will monitor and scan file system activity in locations that you can specify.

With the code up on GitHub the {brand-plugin-type} is readly extensible but can also be used out of the box with default configuration suitable for common scenarios.

The tool is open source so you can modify as you wish.
Also see [Docker File Watcher](/plugin-list/plugin-secplugs-container-plugins-docker-file-watcher)


## Overview

This {brand-plugin-type} does  'lazy' On-Accessscanning for docker containers. 
There are official packages for Debian, Ubuntu, Fedora,
CentOS and Alpine. This {brand-plugin-type} is a 64-bit native binary that is created with
the requirements of a container environment in mind - small binary size, a
very log memory footprint and quick scan times. This document shows how this
{brand-plugin-type} can be used from all the supported Linux distributions

### Use Your Own API Key
To use additional features and the privacy of your own account, after registering with {brand-name}, sign in with your username and [create an API key](docs?doc=docs/HowTo/CreateKey) 

After creating a key, specify it as the key to use in the Dockerfile

```dockerfile
ENV SECPLUGS_API_KEY <copied api key>
```

### Debian/Ubuntu

The following lines need to be added to your Dockerfile for Debian/Ubuntu

```dockerfile
FROM ubuntu:latest
RUN apt install software-properties-common && apt update
RUN apt-add-repository -y ppa:secplugs/ppa
RUN apt update && apt install secplugs-container-plugin
```

The above pulls the latest `ubuntu` image from docker hub and adds the secplugs ppa repository to the list of repositories. The `apt-add-repository` utility needs `software-properties-common` and hence that gets installed first.

Likewise, for Debian, the following lines need to be added to your Dockerfile

```dockerfile
FROM debian:latest
RUN apt install software-properties-common && apt update
RUN apt-key adv --keyserver <keyserver> --recv <key> && apt-add-repository -y 'deb https://packages.secplugs.com/debian/ stable main'
RUN apt update && apt install secplugs-container-plugin
```

The above pulls the latest debian image from docker hubs and adds the secplugs
debian repository and its gpg keys to the container.

Both ubuntu and debian packages add the container plugin to be run in the
background as a systemd service

### Fedora/CentOS

The following lines need to be added to your Dockerfile for Fedora/CentOS based containers.

```dockerfile
FROM fedora:latest
RUN wget -O /etc/yum.repos.d/secplugs.repo https://packages.secplugs.com/fedora/secplugs.repo
RUN echo <SHA> /etc/yum.repos.d/secplugs.repo | sha256sum --check --status
RUN dnf update && dnf install secplugs-container-plugins
```

### Alpine

Alpine is the most popular Linux distribution for containers owing to its small size. So naturally, we have an alpine package for our secplugs docker plugin. The following lines need to be added  to your Dockerfile for alpine based containers

```dockerfile
FROM alpine:latest
echo "https://alpine.secplugs.com/packages" | tee -a /etc/apk/repositories
apk add secplugs-docker-plugin
rc-update add secplugs-docker-plugin
rc-service secplugs-docker-plugin start
```

### Configuration

The only configuration that is needed for the secplugs-container-plugin is the
`/etc/secplugs/config.json` file. For most common use cases, the only configuration that would be needed is the `watchers` property which tells the plugin which folders to monitor. The default configuration works for most common deployments

```json
{
  "watchers": [
    "/lib",
    "/lib64",
    "/var/lib",
    "/etc/"
  ],
  "api_key": "secplugs-api-key-from-portal"
}
```



## Contact
Having trouble? [Contact {brand-name} ](https://{brand-root-domain}/contacts)