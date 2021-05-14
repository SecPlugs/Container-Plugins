FROM debian:buster
COPY ./secplugs.gpg /tmp/secplugs.gpg
RUN apt update && apt install --yes gnupg ca-certificates
COPY ./secplugs.list /etc/apt/sources.list.d/
RUN  apt-key add /tmp/secplugs.gpg && apt-get update && apt install --yes dockerplugin
COPY start.sh /tmp
RUN chmod +x /tmp/start.sh
#ENTRYPOINT /bin/bash
CMD /tmp/start.sh
