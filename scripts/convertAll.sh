#!/bin/bash

# Change into the script directory
SCRIPTDIR=$(readlink -f $(dirname "$0"))
pushd "${SCRIPTDIR}" > /dev/null

echo "::: Build DataDecoder"
../MonitorHeating/scripts/build.sh "$0"
feedback=$?

if [ 0 -eq ${feedback} ] ; then
    cd ../
    find HeatingData/ -name "*.dat" -exec ./DataDecoder/bin/DataDecoder {} \;
fi

# Back to the original location
popd > /dev/null

exit ${feedback}
