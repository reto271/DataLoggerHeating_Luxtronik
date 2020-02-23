#!/bin/bash

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

# Back to the original location
popd > /dev/null

exit ${feedback}
