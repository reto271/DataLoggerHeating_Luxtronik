#!/bin/bash

CLEAN_ALL=0
CLEAN_ALL_TEXT=""
if [ 1 -eq $# ] ; then
    if [ "--all" == "$1" ] ; then
        CLEAN_ALL=1
        CLEAN_ALL_TEXT="--all"
    fi
fi

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null

echo "Clean MonitorHeating"
../MonitorHeating/scripts/clean.sh "${CLEAN_ALL_TEXT}"
feedback=$?

echo "Clean DataDecoder"
if [ 0 -eq ${feedback} ] ; then
    ../DataDecoder/scripts/clean.sh "${CLEAN_ALL_TEXT}"
    feedback=$?
fi

echo "Clean Tests"
if [ 0 -eq ${feedback} ] ; then
    ../Test/scripts/clean.sh "${CLEAN_ALL_TEXT}"
    feedback=$?
fi

echo "Clean Utils"
if [ 0 -eq ${feedback} ] ; then
    ../Utils/scripts/clean.sh "${CLEAN_ALL_TEXT}"
    feedback=$?
fi

cd ..
if [ 1 -eq ${CLEAN_ALL} ] ; then
    rm -vrf GPATH GTAGS GRTAGS
fi

# Back to the original location
popd > /dev/null

exit ${feedback}
