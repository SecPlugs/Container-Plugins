#!/bin/bash

tar xf setup.tgz
cp ./secplugs.gpg /tmp/secplugs.gpg
apt update && apt install --yes gnupg ca-certificates
sed -i 's/debian/ubuntu/;s/buster/focal/;' /tmp/secplugs.list
cp ./secplugs.list /etc/apt/sources.list.d/
apt-key add /tmp/secplugs.gpg && apt-get update && apt install --yes dockerplugin
