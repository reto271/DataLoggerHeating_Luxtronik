#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null

echo "Build MonitorHeating"
../MonitorHeating/scripts/build.sh "$0"
feedback=$?

echo "Build DataDecoder"
if [ 0 -eq ${feedback} ] ; then
    ../DataDecoder/scripts/build.sh "$0"
    feedback=$?
fi

echo "Build Test"
if [ 0 -eq ${feedback} ] ; then
    if [ ! -d "../Test/googletest" ] ; then
        echo "Get google test framework"
        ../Test/scripts/getGoogleTest.sh
    fi
    ../Test/scripts/build.sh "$0"
    feedback=$?
fi

echo "Build Utils"
if [ 0 -eq ${feedback} ] ; then
    ../Utils/scripts/build.sh "$0"
    feedback=$?
fi

# Back to the original location
popd > /dev/null

exit ${feedback}
