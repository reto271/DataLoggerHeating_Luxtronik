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

rm -vrf docu
find -name "*~" -exec rm -v {} \;
rm -rf bin

if [ 1 -eq ${CLEAN_ALL} ] ; then
    rm -vrf tmp googletest debug
    rm -vrf GPATH GTAGS GRTAGS
fi
echo "$0 done"

# Back to the original location
popd > /dev/null
