#!/bin/bash

DIR=$(mktemp -d /tmp/dockerpluginXXXX)
MYPATH=$(dirname $0)
VERSION=0.1.0
SRC=$MYPATH/dockerplugin
DST=dockerplugin-${VERSION}
SRCTAR=dockerplugin-0.1.0.tar.gz

cp -a  $SRC $DIR/$DST
CWD=$(pwd)
cd $DIR && tar czf $SRCTAR $DST && cd $CWD
cp $DIR/$SRCTAR $MYPATH
test -d $DIR && rm -rf $DIR
