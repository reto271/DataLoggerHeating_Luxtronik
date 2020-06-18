#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null
cd ..

../Common/scripts/showSW_Version.sh

# Offer option -v : verbose
CLEAN_ALL=0
if [ 1 -eq $# ] ; then
    if [ "--all" == "$1" ] ; then
        CLEAN_ALL=1
    fi
fi

rm -rf docu
find -name "*~" -exec rm -f {} \;
rm -rf bin

if [ 1 -eq ${CLEAN_ALL} ] ; then
    rm -rf tmp googletest debug
    rm -rf GPATH GTAGS GRTAGS
fi
echo "$0 done"

# Back to the original location
popd > /dev/null
