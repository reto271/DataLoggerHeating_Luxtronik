#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null
cd ..

# Offer option -v : verbose
CLEAN_ALL=0
if [ 1 -eq $# ] ; then
    if [ "--all" == "$1" ] ; then
        CLEAN_ALL=1
    fi
fi

rm -rf bin GPATH GTAGS GRTAGS *~
if [ 1 -eq ${CLEAN_ALL} ] ; then
    rm -rf googletest debug
fi
echo "$0 done"

# Back to the original location
popd > /dev/null
