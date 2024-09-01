#!/bin/sh
name="zsh"
desc="Customizable command shell for UNIX-like environments"
depends="devel,gmake"
#buildopts has the same format as depends
buildopts=""
version="5.9"
source="https://www.zsh.org/pub/zsh-$version.tar.xz"

. /etc/luna/make.conf

srcdir=$(luna --fetch-tarball $source)
cd "$srcdir/zsh-$version"
./configure
make $MAKEFLAGS
DESTDIR=$(luna --make-staging-directory $name)
make install