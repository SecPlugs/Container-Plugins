#!/bin/bash

PKGDIR=$(mktemp -d /tmp/pkgXXXX)
MYPATH=$(dirname $0)
VERSION=$(cat $MYPATH/version)
SRC_TAR=dockerplugin-${VERSION}.tar.gz
CWD=$(pwd)
BUILDDIR=$CWD/pkg
test -d $BUILDDIR || mkdir $BUILDDIR
rm -rf $BUILDDIR/*

cp $SRC_TAR $PKGDIR
cp -r packaging/debian/template $PKGDIR
cd $PKGDIR
tar xf $SRC_TAR
cd dockerplugin-${VERSION}
dh_make -s -y -f ../$SRC_TAR
cp -r $PKGDIR/template/* debian/
rm debian/compat
dpkg-buildpackage -uc -us
cp $PKGDIR/*.{deb,dsc,changes,buildinfo} $BUILDDIR/
cd $CWD
rm -rf $PKGDIR
