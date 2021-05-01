#!/bin/bash

PKGDIR=$(mktemp -d /tmp/pkgXXXX)
SRC_TAR=dockerplugin-0.1.0.tar.gz
CWD=$(pwd)

cp $SRC_TAR $PKGDIR
cp -r packaging/debian/template $PKGDIR
cd $PKGDIR
tar xf $SRC_TAR
cd dockerplugin-0.1.0
dh_make -s -y -f ../$SRC_TAR
cp -r $PKGDIR/template/* debian/
dpkg-buildpackage -uc -us
cp $PKGDIR/*.{deb,dsc,changes,buildinfo} $CWD
cd $CWD
rm -rf $PKGDIR
