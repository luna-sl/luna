#!/bin/sh
# this is REQUIRED
set -e
name="zsh"
desc="Customizable command shell for UNIX-like environments"
depends="devel,gmake"
#buildopts has the same format as depends
buildopts=""
version="5.9"
source="https://www.zsh.org/pub/zsh-$version.tar.xz"

#as is this
. /etc/luna/make.conf

srcdir=$(./build/luna --fetch-tarball $source)
export DESTDIR=$(./build/luna --make-staging-directory $name)
cd "$srcdir/zsh-$version"
./configure --prefix=/usr/
make $MAKEFLAGS
make install