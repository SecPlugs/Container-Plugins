
A brief documentation on how to use this plugin

### Overview

This plugin does  'lazy' On-Accessscanning for docker containers. 
There are official packages for Debian, Ubuntu, Fedora,
CentOS and Alpine. This plugin is a 64-bit native binary that is created with
the requirements of a container environment in mind - small binary size, a
very log memory footprint and quick scan times. This document shows how this
plugin can be used from all the supported Linux distributions

### Plugin Registration

1. Create a Dockerfile that installs the container-plugin. Some common distributions are explained below
2. From {sp} console Plugin Management page, register your `container-plugin`
3. The registration will provide an API key that can be copied
4. Copy the API key and the following line to your Dockerfile

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

