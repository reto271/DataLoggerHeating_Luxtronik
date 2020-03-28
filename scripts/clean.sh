#!/bin/bash

DELETE_ALL=0
if [ 1 -eq $# ] ; then
    PARA1=$1
    if [ "${PARA1}" = "--all" ] ; then
        DELETE_ALL=1
    fi
fi


# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null

echo "Clean MonitorHeating"
../MonitorHeating/scripts/clean.sh "$0"
feedback=$?

echo "Clean DataDecoder"
if [ 0 -eq ${feedback} ] ; then
    ../DataDecoder/scripts/clean.sh "$0"
    feedback=$?
fi

echo "Clean Tests"
if [ 0 -eq ${feedback} ] ; then
    ../Test/scripts/clean.sh "$0"
    feedback=$?
fi

echo "Clean Utils"
if [ 0 -eq ${feedback} ] ; then
    ../Utils/scripts/clean.sh "$0"
    feedback=$?
fi

cd ..
if [ 1 -eq ${DELETE_ALL} ] ; then
    rm -vrf GPATH GTAGS GRTAGS
fi

# Back to the original location
popd > /dev/null

exit ${feedback}
