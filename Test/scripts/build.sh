#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null

../../Common/scripts/showSW_Version.sh

# Offer option -v : verbose
VERBOSE=0
if [ 1 -eq $# ] ; then
    if [ "-v" == "$1" ] ; then
        VERBOSE=1
    fi
fi

# Cmake followed by make...
mkdir -p ../bin
cd ../bin
cmake ..
if [ 1 -eq ${VERBOSE} ] ; then
    make VERBOSE=1
else
    make
fi
feedback=$?

# Back to the original location
popd > /dev/null

exit ${feedback}
