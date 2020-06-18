#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null
cd ..

./Common/scripts/showSW_Version.sh

#gloabl -u does not correctly remove old symbols, hence delete the gtags files and create them anew.
#global -uv &
rm -f GPATH  GRTAGS  GTAGS
gtags &
sleep 1

emacs Common/src/ProgramVersion.hpp &

# Back to the original location
popd > /dev/null
