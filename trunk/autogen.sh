#!/bin/bash

die() {
    echo "$@" >&2
    exit 1
}

_libtoolize() {
    platform=$(uname|sed -e s/_.*//|tr '[:upper:]' '[:lower:]')
    if [ "$platform" == "darwin"  ];
    then
        glibtoolize --automake --copy
    else
        libtoolize --automake --copy
    fi
}

echo ">>> libtoolize --automake --copy"
_libtoolize || die "libtoolize failed"

echo ">>> aclocal -I m4"
aclocal -I m4 || die "aclocal failed"

echo ">>> automake --add-missing --copy"
automake --add-missing --copy || die "automake failed"

echo ">>> autoconf"
autoconf || die "autoconf failed"
