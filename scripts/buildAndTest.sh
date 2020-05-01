#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null

echo "::: Build MonitorHeating"
../MonitorHeating/scripts/build.sh "$0"
feedback=$?

if [ 0 -eq ${feedback} ] ; then
    echo "::: Build DataDecoder"
    ../DataDecoder/scripts/build.sh "$0"
    feedback=$?
fi

if [ 0 -eq ${feedback} ] ; then
    echo "::: Build Test"
    if [ ! -d "../Test/googletest" ] ; then
        echo "Get google test framework"
        ../Test/scripts/getGoogleTest.sh
    fi
    ../Test/scripts/build.sh "$0"
    feedback=$?
fi

if [ 0 -eq ${feedback} ] ; then
    echo "::: Build Utils"
    ../Utils/scripts/build.sh "$0"
    feedback=$?
fi


if [ 0 -eq ${feedback} ] ; then
    echo "::: Run the Tests"
    cd ..
    ./Test/bin/Test_HeatingDataMonitorAndConverter
    feedback=$?
fi


# Back to the original location
popd > /dev/null

exit ${feedback}
