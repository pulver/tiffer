#!/bin/bash

# Git add, commit, and tag VERSION and $VERSION_H files.
# $VERSION_H is created from VERSION and can be #included so programs know their versions.

# const char* $STRING_NAME variable will be set at #include "version.h".
STRING_NAME=TIFFER_VERSION

SRC_DIR=src
VERSION_H=$SRC_DIR/version.h

if [ ! -s VERSION ]
then
    echo "Please create a non-empty single-line text file VERSION with the desired tag/version number."
    exit 1
fi

# Trailing newlines are deleted from file.
VERSION=`cat VERSION`

# Check VERSION is single-line.
if [ $(echo -n "$VERSION" | wc -l) != '0' ]
then
    echo "Error: The VERSION file must be a single-line file with the desired tag/version number."
    exit 1
fi

# Check VERSION doesn't already exist as a tag.
if [ -n "$(git tag -l | grep -Fx "$VERSION")" ]
then
    echo "Error: A tag with version($VERSION) already exists."
    exit 1
fi

# Create $SRC_DIR if necessary.
if ! mkdir -p $SRC_DIR
then
    exit 1
fi

# Create $SRC_DIR if necessary.
echo "const char $STRING_NAME[] = \"$VERSION\";" > $VERSION_H

# Update $VERSION_H
echo "Contents of $VERSION_H:"
echo
cat $VERSION_H
echo

# Git add, commit, and tag.
git add VERSION $VERSION_H
git commit -m "Tag version $VERSION" VERSION $VERSION_H
git tag "$VERSION"
